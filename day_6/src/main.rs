use std::io::BufRead;

fn main() {
    let f = match std::fs::File::open("input.txt") {
        Ok(f) => f,
        Err(err) => panic!("Problem opening input file: {err:?}"),
    };

    let r = std::io::BufReader::new(f);

    let mut area: Vec<String> = Vec::new();
    for ln in r.lines() {
        let ln = ln.unwrap();
        area.push(ln);
    }

    let part_1 = patrol_area(area.clone());
    let part_2 = 0;

    println!("{}", part_1);
    println!("{}", part_2);
}

fn patrol_area(area: Vec<String>) -> i32 {
    let mut ans = 1;

    let (mut x, mut y, d) = find_guard(area.clone());
    if x >= area.len() {
        return -1;
    }

    let (mut dirx, mut diry) = match d {
        '<' => (0, -1),
        '>' => (0, 1),
        '^' => (-1, 0),
        _ => (1, 0),
    };

    let mut area = area;
    let mut d = d.to_string();
    loop {
        let m = (x as i32) + dirx;
        let n = (y as i32) + diry;
        if m < 0 || m >= area.len() as i32 || n < 0 || n >= area[0].len() as i32 {
            break;
        }

        let nx = m as usize;
        let ny = n as usize;

        let c = area[nx].chars().nth(ny).unwrap();
        if c == '#' {
            (dirx, diry, d) = turn_right(dirx, diry);
            continue;
        } else if c == '.' {
            ans += 1;
            area[nx].replace_range(ny..ny + 1, &d);
        }

        x = nx;
        y = ny;
    }

    for row in area {
        println!("{}", row);
    }

    ans
}

fn find_guard(area: Vec<String>) -> (usize, usize, char) {
    for (i, row) in area.iter().enumerate() {
        for (j, c) in row.chars().enumerate() {
            if c == '<' || c == '>' || c == 'v' || c == '^' {
                return (i, j, c);
            }
        }
    }

    (area.len(), 0, '\0')
}

fn turn_right(dirx: i32, diry: i32) -> (i32, i32, String) {
    match (dirx, diry) {
        (0, -1) => (-1, 0, String::from("^")),
        (-1, 0) => (0, 1, String::from(">")),
        (0, 1) => (1, 0, String::from("v")),
        _ => (0, -1, String::from("<")),
    }
}
