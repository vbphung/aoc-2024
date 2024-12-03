use core::panic;
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
        let mut v: Vec<i32> = Vec::new();

        let ln = ln.unwrap();
        for w in ln.split_whitespace() {
            let n: i32 = w.parse().unwrap();
            v.push(n);
        }

        let i = check_safe(v.clone());
        if i <= 0 {
            part_1 += 1;
            part_2 += 1;
        } else {
            for j in -2..1 {
                if check_safe_without_index(v.clone(), i + j) {
                    part_2 += 1;
                    break;
                }
            }
        }
    }

    println!("{}", part_1);
    println!("{}", part_2);
}

fn check_safe(v: Vec<i32>) -> i32 {
    if v.len() <= 1 {
        return -1;
    }

    let mut inc = 0;
    for i in 1..v.len() {
        let d = v[i] - v[i - 1];
        if d * inc < 0 || d.abs() < 1 || d.abs() > 3 {
            return i as i32;
        }
        inc = d;
    }

    -1
}

fn check_safe_without_index(v: Vec<i32>, i: i32) -> bool {
    if i < 0 || (i as usize) >= v.len() {
        return false;
    }

    let mut tol = v.to_vec();
    tol.remove(i as usize);

    check_safe(tol) <= 0
}
