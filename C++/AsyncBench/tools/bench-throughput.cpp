#include <pthread.h>
#include <cstdint>
#include <atomic>
#include <barrier>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <thread>

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/core/noncopyable.hpp>
#include <boost/asio/experimental/as_single.hpp>
#include <boost/program_options.hpp>

namespace asio = boost::asio;
namespace this_coro = boost::asio::this_coro;
namespace po = boost::program_options;

using boost::system::error_code;
using asio::ip::tcp;
using boost::asio::awaitable;
using asio::co_spawn;
using asio::detached;
using asio::use_awaitable;
using default_token = asio::experimental::as_single_t<asio::use_awaitable_t<>>;
using tcp_acceptor = default_token::as_default_on_t<tcp::acceptor>;
using tcp_socket = default_token::as_default_on_t<tcp::socket>;

#if defined(BOOST_ASIO_ENABLE_HANDLER_TRACKING)
#define use_awaitable \
    boost::asio::use_awaitable_t(__FILE__, __LINE__, __PRETTY_FUNCTION__)
#endif

/* Number of workers (threads). */
static uint32_t num_workers = 1;

/* Number of connections per worker. */
static uint32_t num_conns = 1;

/* Number of requests per connections. */
static uint32_t num_reqs = 1;

/* Barrier to wait until all threads are initialized, so that we can start to measure the time */
static pthread_barrier_t start_barrier;

/* Barrier to wait until all threads finished their work */
static pthread_barrier_t end_barrier;

static uint64_t time_diff;

static tcp::endpoint endpoint;

static std::string REQUEST {"Hello, World!"};

class connection : private boost::noncopyable {
public:
    explicit connection(asio::io_context &ctx)
        : socket_(std::move(tcp::socket(ctx))) {}

    void connect() {
        error_code code;
        socket_.connect(endpoint, code);
        if (code) {
            std::cerr << "Connecting to the server failed: " << code.message() << "\n";
            exit(1);
        }
    }

    awaitable<void> run() {
        error_code code;
        std::string read_data(REQUEST.length(), '\0');

        for (uint32_t i = 0; i < num_reqs; i++) {
            co_await socket_.async_send(asio::const_buffer(REQUEST.c_str(), REQUEST.length()));
            co_await socket_.async_receive(asio::mutable_buffer(read_data.data(), read_data.length()));
        }
        socket_.shutdown(tcp::socket::shutdown_both);
        socket_.close(code);
        co_return;
    }
private:
    tcp_socket socket_;
};

static inline uint64_t get_current_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    const uint64_t nsecs_per_sec = 1000 * 1000 * 1000;
    return (uint64_t)ts.tv_sec * nsecs_per_sec + (uint64_t)ts.tv_nsec;
}

__attribute__((aligned(64), noinline))
static void worker_thread(uint32_t worker_id) {
    asio::io_context ctx;
    std::vector<std::unique_ptr<connection>> conns;
    conns.reserve(num_conns);

    for (uint32_t i = 0; i < num_conns; i++) {
        conns.emplace_back(std::make_unique<connection>(ctx));
        conns.back()->connect();
    }

    // wait for all threads to finish the initialization
    pthread_barrier_wait(&start_barrier);

    uint64_t start = 0, end = 0;

    if (worker_id == 0) {
        start = get_current_ns();
    }

    for (auto && conn : conns) {
        co_spawn(ctx, conn->run(), detached);
    }
    ctx.run();

    // wait for all threads to finish work
    pthread_barrier_wait(&end_barrier);

    if (worker_id == 0) {
        end = get_current_ns();
        time_diff = end - start;
    }
}

static void usage(const char *program_name, const po::options_description &desc) {
    std::cout << "usage: " << program_name << " ip port\n\n";
    std::cout << desc << "\n";
}

int main(int argc, char *argv[]) {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("num-conns,c", po::value<uint32_t>(&num_conns), "Number of connections per worker (default 1)")
        ("num-reqs,r", po::value<uint32_t>(&num_reqs), "Number of requests per connections (default 1)")
        ("num-workers,w", po::value<uint32_t>(&num_workers), "Number of worker threads (default 1)");

    std::string host;
    uint16_t port = 0;
    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("HOST-IPV4", po::value<std::string>(&host), "Server ipv4 address")
        ("HOST-PORT", po::value<uint16_t>(&port), "Server port");

    po::options_description cmdline_options;
    cmdline_options.add(desc).add(hidden);

    po::positional_options_description p;
    p.add("HOST-IPV4", 1);
    p.add("HOST-PORT", 1);

    po::variables_map vm;
    try {
        po::store(
            po::command_line_parser(argc, argv)
                .options(cmdline_options)
                .positional(p)
                .run(),
            vm);
        po::notify(vm);
    } catch (const std::exception &e) {
        usage(argv[0], desc);
        return 1;
    }

    if (vm.count("help")) {
        usage(argv[0], desc);
        return 0;
    }
    if (!vm.count("HOST-IPV4") || !vm.count("HOST-PORT")) {
        usage(argv[0], desc);
        return 1;
    }

    try {
        endpoint = tcp::endpoint(asio::ip::make_address(host), port);
    } catch (const std::exception &) {
        std::cout <<  "Invalid <host:port>: " << host << ":" << port << '\n';
        return 1;
    }

    pthread_barrier_init(&start_barrier, nullptr, num_workers);
    pthread_barrier_init(&end_barrier, nullptr, num_workers);

    std::vector<std::thread> workers;
    workers.reserve(num_workers);
    for (uint32_t i = 0; i < num_workers; i++) {
        workers.emplace_back(worker_thread, i);
    }

    for (auto &&w : workers) {
        w.join();
    }

    /* Calculate and print results */
    uint64_t total_requests = static_cast<uint64_t>(num_conns) * static_cast<uint64_t>(num_reqs);
    if (total_requests > UINT64_MAX / static_cast<uint64_t>(num_workers)) {
        std::cerr << "Overflow in the calculation of total requests\n";
        return 1;
    }

    total_requests *= num_workers;
    double delta = static_cast<double>(time_diff) / 1e9;
    std::cout << total_requests << " requests in " << delta << "s, rate: "
        << static_cast<double>(total_requests) / delta << " req/s\n";
    return 0;
}
