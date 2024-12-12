use std::io::BufRead;

fn main() {
    let f = match std::fs::File::open("input.txt") {
        Ok(f) => f,
        Err(err) => panic!("Problem opening input file: {err:?}"),
    };

    let r = std::io::BufReader::new(f);

    let mut part_1 = 0;
    let mut part_2 = 0;

    for ln in r.lines() {
        let ln = ln.unwrap().replace(":", "");
        let ns: Vec<i64> = ln.split(' ').map(|x| x.parse().unwrap()).collect();

        let n = ns[0];
        let ns = ns[1..].to_vec();

        if check_calibration(n, &ns) {
            part_1 += n;
        }

        if check_calibration_with_concatenation(n, &ns) {
            part_2 += n;
        }
    }

    println!("{}", part_1);
    println!("{}", part_2);
}

fn check_calibration(n: i64, ns: &[i64]) -> bool {
    let l = ns.len() - 1;
    let mut ops = vec![0; l];
    let all = 2_usize.pow(l as u32);
    for i in 0..all {
        let mut j = i;
        for op in ops.iter_mut() {
            *op = j % 2;
            j /= 2;
        }

        if cal(ns, &ops) == n {
            return true;
        }
    }

    false
}

fn cal(ns: &[i64], ops: &[usize]) -> i64 {
    let mut ans = ns[0];

    for (i, &n) in ns.iter().skip(1).enumerate() {
        ans = if ops[i] == 0 { ans + n } else { ans * n };
    }

    ans
}

fn check_calibration_with_concatenation(n: i64, ns: &[i64]) -> bool {
    let l = ns.len() - 1;
    let mut ops = vec![0; l];
    let all = 3_usize.pow(l as u32);
    for i in 0..all {
        let mut j = i;
        for op in ops.iter_mut() {
            *op = j % 3;
            j /= 3;
        }

        if cal_with_concatenation(ns, &ops) == n {
            return true;
        }
    }

    false
}

fn cal_with_concatenation(ns: &[i64], ops: &[usize]) -> i64 {
    let mut ans = ns[0];
    for (i, &n) in ns.iter().skip(1).enumerate() {
        ans = if ops[i] == 0 {
            ans + n
        } else if ops[i] == 1 {
            ans * n
        } else {
            ans * n_len(n) + n
        };
    }

    ans
}

fn n_len(mut n: i64) -> i64 {
    let mut ans = 1;
    while n > 0 {
        ans *= 10;
        n /= 10;
    }

    ans
}
