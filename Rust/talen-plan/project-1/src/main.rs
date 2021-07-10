extern crate clap;

use clap::{App, Arg, SubCommand};

fn main() {
    let matches = App::new("kvs")
        .version(env!("CARGO_PKG_VERSION"))
        .subcommand(
            SubCommand::with_name("get")
                .about("Get the string value of a given key string")
                .arg(Arg::with_name("key").required(true)),
        )
        .subcommand(
            SubCommand::with_name("set")
                .about("Set the string value of a given key string")
                .arg(Arg::with_name("key").required(true))
                .arg(Arg::with_name("value").required(true)),
        )
        .subcommand(
            SubCommand::with_name("rm")
                .about("Remove a given key string")
                .arg(Arg::with_name("key").required(true)),
        )
        .get_matches();

    match matches.subcommand() {
        ("get", Some(_)) => {
            eprintln!("unimplemented");
            std::process::exit(-1);
        }
        ("set", Some(_)) => {
            eprintln!("unimplemented");
            std::process::exit(-1);
        }
        ("rm", Some(_)) => {
            eprintln!("unimplemented");
            std::process::exit(-1);
        }
        _ => unreachable!(),
    }
}
