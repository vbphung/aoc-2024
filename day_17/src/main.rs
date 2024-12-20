use std::io::BufRead;

fn main() {
    let f = match std::fs::File::open("input.txt") {
        Ok(f) => f,
        Err(err) => panic!("Problem opening input file: {err:?}"),
    };

    let r = std::io::BufReader::new(f);
    let lns: Vec<String> = r.lines().map(|x| x.unwrap()).collect();

    let a: i32 = lns[0].split(' ').collect::<Vec<&str>>()[2].parse().unwrap();
    let b: i32 = lns[1].split(' ').collect::<Vec<&str>>()[2].parse().unwrap();
    let c: i32 = lns[2].split(' ').collect::<Vec<&str>>()[2].parse().unwrap();

    let program = lns[4].split(' ').collect::<Vec<&str>>()[1]
        .split(',')
        .collect::<Vec<&str>>()
        .iter()
        .map(|x| x.parse().unwrap())
        .collect::<Vec<i32>>();
    let opcodes = program
        .chunks(2)
        .map(|x| x.to_vec())
        .collect::<Vec<Vec<i32>>>();

    println!("Registers: a({a}) b({b}) c({c})");
    println!("Program: {:?}", program);
    println!("Opcodes: {:?}", opcodes);

    let part_1 = run_program(a, b, c, &opcodes);
    let part_2 = reverse_engine(&program, 0, program.len() as i32 - 1);

    println!("{}", part_1);
    println!("{}", part_2);
}

// Shout out to this explanation https://youtu.be/y-UPxMAh2N8?si=1Mh3cPIErBZcY7Zv
// which saved my brain from melting
fn reverse_engine(program: &Vec<i32>, ans: u64, cur: i32) -> u64 {
    if cur < 0 {
        return ans;
    }

    let cur = cur as usize;

    for b in 0..8 {
        let a = (ans << 3) + b;
        let b = b ^ 5;
        let c = a >> b;
        let b = b ^ 6;

        // My program has (0, 3) opcode here
        // let a = a >> 3;
        // But we mustn't decrease `a` here
        // because we're backtracking its value

        let b = b ^ c;
        if (b % 8) as i32 == program[cur] {
            let sub = reverse_engine(program, a, cur as i32 - 1);
            if sub != 0 {
                return sub;
            }
        }
    }

    0
}

fn run_program(a: i32, b: i32, c: i32, opcodes: &[Vec<i32>]) -> String {
    let mut ip = 0;

    let mut a = a;
    let mut b = b;
    let mut c = c;

    let mut res = vec![];

    while ip < opcodes.len() {
        let op = &opcodes[ip];
        let combo = get_combo_operand(a, b, c, op[1]) as u32;
        match op[0] {
            0 => {
                a /= i32::pow(2, combo);
            }
            1 => {
                b ^= op[1];
            }
            2 => {
                b = (combo % 8) as i32;
            }
            3 => {
                ip = if a != 0 { op[1] as usize } else { ip + 1 };
            }
            4 => {
                b ^= c;
            }
            5 => {
                let out = combo % 8;
                res.push(out);
            }
            6 => {
                b = a / i32::pow(2, combo);
            }
            7 => {
                c = a / i32::pow(2, combo);
            }
            _ => panic!("Invalid opcode: {}", op[0]),
        }

        if op[0] != 3 {
            ip += 1;
        }
    }

    res.iter()
        .map(|out| out.to_string())
        .collect::<Vec<String>>()
        .join(",")
}

fn get_combo_operand(a: i32, b: i32, c: i32, operand: i32) -> i32 {
    match operand {
        0_i32..=3_i32 => operand,
        4 => a,
        5 => b,
        6 => c,
        _ => 0,
    }
}
