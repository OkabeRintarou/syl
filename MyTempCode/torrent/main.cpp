#include <iostream>
#include <string>
#include <fstream>
#include "Error.h"
#include "BTParser.h"
#include "BitTorrent.h"
#include "nettools.h"
#include <netdb.h>

int main()
{
	std::ios_base::sync_with_stdio(false);


	try{

		BitTorrent torrent("ubuntu.torrent");
		std::cout << "announce: " << torrent.getAnnounce() << std::endl;
		std::cout << "file size:" << static_cast<double>(torrent.getFileLength()) / 1024.0 / 1024.0  << "MB" << std::endl;
		std::cout << "comment:" << torrent.getComment() << std::endl;
		std::cout << "file name:" << torrent.getFileName() << std::endl;
		const auto & pieces = torrent.getPieces();
		
		struct hostent * hptr = gethostbyname("http://torrent.ubuntu.com:6969");

		if(hptr == 0){
			perror("gethostbyname error");
		}
		else{
			std::cout << "Hello,WOrld" << std::endl;
		}
		//int clientfd = open_clientfd(torrent.getAnnounce().c_str(),80);

		//td::cout << clientfd << std::endl;


		/*std::fstream in("demo.torrent", std::ios::in | std::ios::binary | std::ios::ate);

		char * contents = nullptr;
		ListType L;
		DictionaryType D;

		if (in.is_open()){

		size_t size = in.tellg();
		contents = new char[size + 1];
		in.seekg(0, std::ios::beg);
		in.read(contents, size);
		contents[size] = 0;
		in.close();

		parseDictionary(contents, L, D);

		}		*/
	}
	catch (const std::exception & e){
		std::cerr << e.what() << std::endl;
	}

	return 0;
}