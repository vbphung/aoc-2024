use std::{collections::HashMap, io::BufRead};

struct Operation {
    pub op: String,
    pub a: String,
    pub b: String,
}

fn main() {
    let f = match std::fs::File::open("input.txt") {
        Ok(f) => f,
        Err(err) => panic!("Problem opening input file: {err:?}"),
    };

    let r = std::io::BufReader::new(f);

    let mut vals: HashMap<String, i32> = HashMap::new();
    let mut ops: HashMap<String, Operation> = HashMap::new();

    let mut broke = false;
    for ln in r.lines() {
        let ln = ln.unwrap();
        if ln.is_empty() {
            broke = true;
            continue;
        }

        if !broke {
            let parts = ln.split(": ").collect::<Vec<&str>>();
            vals.insert(parts[0].to_string(), parts[1].parse().unwrap());
        } else {
            let parts = ln.split(" ").collect::<Vec<&str>>();
            ops.insert(
                parts[parts.len() - 1].to_string(),
                Operation {
                    op: parts[1].to_string(),
                    a: parts[0].to_string(),
                    b: parts[2].to_string(),
                },
            );
        }
    }

    let t = std::time::Instant::now();

    let part_1 = output_from_zs(&mut vals, &ops);

    let dur = t.elapsed();

    println!("{}", part_1);
    println!("Took {:?}ms", dur.as_millis());
}

fn output_from_zs(vals: &mut HashMap<String, i32>, ops: &HashMap<String, Operation>) -> u64 {
    let mut most_z: String = String::new();
    for (var, _) in ops.iter() {
        most_z = most_z.max(var.clone());
    }

    let mut ans: u64 = 0;

    let most_bit: i32 = most_z[1..].parse().unwrap();
    for b in (0..=most_bit).rev() {
        let z = format!("z{:02}", b);
        let val = get_val(vals, ops, z);
        ans = (ans << 1) | (val as u64);
    }

    ans
}

fn get_val(vals: &mut HashMap<String, i32>, ops: &HashMap<String, Operation>, var: String) -> i32 {
    if let Some(val) = vals.get(&var) {
        return *val;
    }

    let op = ops.get(&var).unwrap();
    let a = get_val(vals, ops, op.a.clone());
    let b = get_val(vals, ops, op.b.clone());

    vals.insert(
        var.clone(),
        match op.op.as_str() {
            "AND" => a & b,
            "OR" => a | b,
            _ => a ^ b,
        },
    );

    *vals.get(&var).unwrap()
}
