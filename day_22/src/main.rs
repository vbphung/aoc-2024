use std::{
    collections::{HashMap, HashSet},
    io::BufRead,
};

fn main() {
    let f = match std::fs::File::open("input.txt") {
        Ok(f) => f,
        Err(err) => panic!("Problem opening input file: {err:?}"),
    };

    let r = std::io::BufReader::new(f);

    let mut initials: Vec<u64> = Vec::new();
    for ln in r.lines() {
        let ln = ln.unwrap();
        initials.push(ln.parse().unwrap());
    }

    let mut part_1: u64 = 0;

    let mut prices: Vec<Vec<i32>> = Vec::new();
    let mut changes: Vec<Vec<i32>> = Vec::new();

    let t = std::time::Instant::now();

    for init_secret in &initials {
        let (secret, p, c) = secret_seq(*init_secret, 2000);
        prices.push(p);
        changes.push(c);
        part_1 += secret;
    }

    let mut all_seqs: HashMap<i32, i32> = HashMap::new();
    for i in 0..initials.len() {
        buyer_seqs(&mut all_seqs, &prices[i], &changes[i], 2000);
    }

    let part_2 = all_seqs.values().max().unwrap();

    let dur = t.elapsed();

    println!("{}", part_1);
    println!("{}", part_2);
    println!("Took {:?}ms", dur.as_millis());
}

fn buyer_seqs(all_seqs: &mut HashMap<i32, i32>, prices: &[i32], changes: &[i32], k: usize) {
    let mut seen: HashSet<i32> = HashSet::new();
    let mut seq = vec![changes[0], changes[1], changes[2], changes[3]];
    for i in 3..k {
        let h = hash_seq(&seq);
        if !seen.contains(&h) {
            *all_seqs.entry(h.clone()).or_insert(0) += prices[i];
            seen.insert(h);
        }

        seq.remove(0);
        seq.push(changes[(i + 1) % changes.len()]);
    }
}

fn hash_seq(seq: &[i32]) -> i32 {
    seq.iter().fold(0, |acc, &x| acc * 100 + (10 + x))
}

fn secret_seq(init_secret: u64, k: usize) -> (u64, Vec<i32>, Vec<i32>) {
    let mut prices: Vec<i32> = vec![0; k];
    let mut changes: Vec<i32> = vec![0; k];

    let mut cur = init_secret;
    for i in 0..k {
        let next = next_secret(cur);
        prices[i] = (next % 10) as i32;
        changes[i] = prices[i] - (cur % 10) as i32;
        cur = next;
    }

    (cur, prices, changes)
}

fn next_secret(secret: u64) -> u64 {
    let n = secret * 64;
    let secret = mix(secret, n);
    let secret = prune(secret);
    let n = secret / 32;
    let secret = mix(secret, n);
    let secret = prune(secret);
    let n = secret * 2048;
    let secret = mix(secret, n);
    prune(secret)
}

fn mix(secret: u64, n: u64) -> u64 {
    n ^ secret
}

fn prune(n: u64) -> u64 {
    n % 16777216
}
