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

    let t = std::time::Instant::now();
    let part_2 = stuck_poss(area);
    let dur = t.elapsed();

    println!("{}", part_1);
    println!("{}", part_2);
    println!("Part 2 took {:?}ms", dur.as_millis());
}

fn patrol_area(area: Vec<String>) -> i32 {
    let mut ans = 1;

    let (mut x, mut y, d) = find_guard(area.clone());
    if x >= area.len() {
        return -1;
    }

    let (mut dirx, mut diry) = dir(d);
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

    ans
}

fn stuck_poss(area: Vec<String>) -> i32 {
    let mut ans = 0;

    let (mut x, mut y, d) = find_guard(area.clone());
    if x >= area.len() {
        return -1;
    }

    let (mut dirx, mut diry) = dir(d);
    let mut a = area;
    let mut d = d.to_string();

    loop {
        let m = (x as i32) + dirx;
        let n = (y as i32) + diry;
        if m < 0 || m >= a.len() as i32 || n < 0 || n >= a[0].len() as i32 {
            break;
        }

        let nx = m as usize;
        let ny = n as usize;
        let c = a[nx].chars().nth(ny).unwrap();
        if c == '#' {
            (dirx, diry, d) = turn_right(dirx, diry);
            continue;
        } else if c == '.' {
            a[nx].replace_range(ny..ny + 1, "#");
            if check_loop(a.clone(), x, y, d.chars().next().unwrap()) {
                ans += 1;
            }

            a[nx].replace_range(ny..ny + 1, &d);
        }

        x = nx;
        y = ny;
    }

    ans
}

fn check_loop(area: Vec<String>, x: usize, y: usize, d: char) -> bool {
    let (mut x, mut y) = (x, y);
    let (mut dirx, mut diry) = dir(d);
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
        } else if c == d.chars().next().unwrap() {
            return true;
        } else if c == '.' {
            area[nx].replace_range(ny..ny + 1, &d);
        }

        x = nx;
        y = ny;
    }

    false
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

fn dir(d: char) -> (i32, i32) {
    match d {
        '<' => (0, -1),
        '>' => (0, 1),
        '^' => (-1, 0),
        _ => (1, 0),
    }
}
