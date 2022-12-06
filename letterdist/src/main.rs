use std::collections::HashMap;
use std::fs::File;
use std::io::{self, BufRead};
use std::process;
use std::sync::mpsc;
use std::time::SystemTime;
use std::thread;
use std::os::unix::prelude::FileExt;

use argsplitter::{ArgSplitter, ArgError};
use threadpool::ThreadPool;

enum Method {
    SimpleMain,
    BlockThreads(usize),
    PoolThread(usize),
}

fn main() {
    let mut method = Method::SimpleMain;
    let mut filename = String::new();
    if let Err(error) = parse_args(&mut filename, &mut method) {
        println!("error: {error}");
        process::exit(1);
    }

    println!("file: {filename}");
    let file = File::open(filename).unwrap();

    let time_point = SystemTime::now();

    let letter_distribution = match method {
        Method::SimpleMain => simple(file),
        Method::BlockThreads(threads) => block(file, threads),
        Method::PoolThread(threads) => pool(file, threads)
    };

    let time_point = SystemTime::now().duration_since(time_point).unwrap();

    let mut dist: Vec<(u32, String)> = letter_distribution.into_iter().map(|(s, c)| (c, s)).collect();
    dist.sort_by(|a, b| b.0.cmp(&a.0));

    println!("Time spent: {}", time_point.as_secs_f32());
    println!("Distribuition:");
    println!("{:?}", dist);
}

fn parse_args(file: &mut String, method: &mut Method) -> Result<(), ArgError> {
    let mut argsplitter = ArgSplitter::from_env();

    while let Some(flag) = argsplitter.flag()? {
        match flag {
            "-f" | "--file" => {
                file.clone_from(&argsplitter.param()?)
            },

            "-b" | "--block_threads" => {
                let threads = argsplitter.param()?.parse::<usize>().unwrap();
                *method = Method::BlockThreads(threads);
            },

            "-j" | "--thread_pool" => {
                let threads = argsplitter.param()?.parse::<usize>().unwrap();
                *method = Method::PoolThread(threads);
            },

            "-h" | "--help" => {
                println!("Usage:");
                process::exit(0);
            },
            flag => return Err(ArgError::unknown_flag(flag))
        }
    }

    Ok(())
}

fn simple(file: File) -> HashMap<String, u32> {
    let mut letter_distribution: HashMap<String, u32> = HashMap::new();
    let lines = io::BufReader::new(file).lines();
    for line_result in lines {
        if let Ok(line) = line_result {
            let chars = line.chars();
            for ch in chars {
                let key = if ch.is_alphanumeric() {
                    ch.to_lowercase().to_string()
                } else {
                    String::from("unspecified")
                };
                letter_distribution.entry(key).and_modify(|count| *count += 1).or_insert(1);
            }
        } else {
            println!("error: {:?}", line_result);
            break;
        }
    }

    letter_distribution
}

fn block(file: File, threads: usize) -> HashMap<String, u32> {
    let file_size = file.metadata().unwrap().len();
    let file_chunk_size = file_size as usize / threads;
    let mut v = vec![];
    let mut diff: usize = 0;

    let mut processed: usize = 0;
    for _ in 0..threads {
        let chunk_size = file_chunk_size + diff + 1;
        let mut buff = vec![0u8; chunk_size];
        let readed = file.read_at(&mut buff, (processed) as u64).expect("unable to read from file");
        let ut8str = match String::from_utf8(buff) {
            Ok(s) => {
                diff = 0;
                s
            },
            Err(err) => {
                diff = chunk_size - err.utf8_error().valid_up_to();
                buff = err.into_bytes();
                buff.resize(chunk_size - diff, 0);
                String::from_utf8(buff).unwrap()
            }
        };

        processed += readed - diff;

        let t = thread::spawn(move || {
            let mut letter_distribution: HashMap<String, u32> = HashMap::new();
            let chars = ut8str.chars();
            for ch in chars {
                let key = if ch.is_alphanumeric() {
                        ch.to_lowercase().to_string()
                    } else {
                        String::from("unspecified")
                    };
                letter_distribution.entry(key).and_modify(|count| *count += 1).or_insert(1);
            }

            letter_distribution
        });
        v.push(t);
    }

    let mut letter_distribution: HashMap<String, u32> = HashMap::new();
    for thread in v {
        let dist = thread.join().unwrap();
        for (key, c) in dist {
            letter_distribution.entry(key).and_modify(|count| *count += c).or_insert(c);
        }
    }

    letter_distribution
}

fn pool(file: File, threads: usize) -> HashMap<String, u32> {
    let pool = ThreadPool::new(threads);

    let (tx, rx) = mpsc::channel();

    let lines = io::BufReader::new(file).lines();
    let mut count: usize = 0;
    for line_result in lines {
        if let Ok(line) = line_result {
            let tx = tx.clone();
            pool.execute(move || {
                let mut letter_distribution: HashMap<String, u32> = HashMap::new();
                let chars = line.chars();
                for ch in chars {
                    let key = if ch.is_alphanumeric() {
                        ch.to_lowercase().to_string()
                    } else {
                        String::from("unspecified")
                    };
                    letter_distribution.entry(key).and_modify(|count| *count += 1).or_insert(1);
                }
                tx.send(letter_distribution).expect("send result")
            });
            count += 1;
        } else {
            println!("error: {:?}", line_result);
            break;
        }
    }

    let letter_dist = rx.iter().take(count).fold(HashMap::new(), |mut map, result| {
        for (key, c) in result {
            map.entry(key).and_modify(|count| *count += c).or_insert(c);
        }
        map
    });

    letter_dist
}
