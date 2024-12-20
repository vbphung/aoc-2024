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

    let opcodes = lns[4].split(' ').collect::<Vec<&str>>()[1]
        .split(',')
        .collect::<Vec<&str>>()
        .iter()
        .map(|x| x.parse().unwrap())
        .collect::<Vec<i32>>()
        .chunks(2)
        .map(|x| x.to_vec())
        .collect::<Vec<Vec<i32>>>();

    println!("Registers: a({a}) b({b}) c({c})");
    println!("Opcodes: {:?}", opcodes);

    let part_1 = run_program(a, b, c, &opcodes);

    println!("{}", part_1);
}

fn run_program(a: i32, b: i32, c: i32, opcodes: &Vec<Vec<i32>>) -> String {
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
                a = a / i32::pow(2, combo);
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
                b = b ^ c;
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
