use std::io::BufRead;

fn main() {
    let f = match std::fs::File::open("input.txt") {
        Ok(f) => f,
        Err(err) => panic!("Problem opening input file: {err:?}"),
    };

    let r = std::io::BufReader::new(f);

    let mut part_1 = 0;
    let mut part_2 = 0;
    let mut enabled = true;

    for ln in r.lines() {
        let ln = ln.unwrap();
        part_1 += mul(ln.clone());
        part_2 += mul_with_instructions(ln, &mut enabled);
    }

    println!("{}", part_1);
    println!("{}", part_2);
}

fn mul(ln: String) -> i32 {
    let mut ans = 0;

    let re = regex::Regex::new(r"(mul\((\d+),(\d+)\))").unwrap();
    for (_, [_, a, b]) in re.captures_iter(ln.as_str()).map(|c| c.extract()) {
        let x: i32 = a.parse().unwrap();
        let y: i32 = b.parse().unwrap();
        ans += x * y;
    }

    ans
}

fn mul_with_instructions(ln: String, enabled: &mut bool) -> i32 {
    let mut s = ln.as_str();
    let mut ans = 0;
    let re = regex::Regex::new(r"mul\((\d+),(\d+)\)|don't\(\)|do\(\)").unwrap();

    while let Some(m) = re.find(s) {
        let ms = m.as_str();
        if ms == "do()" {
            *enabled = true;
        } else if ms == "don't()" {
            *enabled = false;
        } else if *enabled {
            if let Some(c) = re.captures(ms) {
                let x: i32 = c.get(1).unwrap().as_str().parse().unwrap();
                let y: i32 = c.get(2).unwrap().as_str().parse().unwrap();
                ans += x * y;
            }
        }

        s = &s[m.end()..];
    }

    ans
}
