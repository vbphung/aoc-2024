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

    let mut edges: HashMap<String, HashSet<String>> = HashMap::new();
    for ln in r.lines() {
        let ln = ln.unwrap();
        let edge = ln.split('-').collect::<Vec<&str>>();
        let u = edge[0].to_string();
        let v = edge[1].to_string();
        (*edges.entry(u.clone()).or_default()).insert(v.clone());
        (*edges.entry(v).or_default()).insert(u);
    }

    let t = std::time::Instant::now();

    let part_1 = set_of_three(&edges);
    let part_2 = largest_set(&edges);

    let dur = t.elapsed();

    println!("{}", part_1);
    println!("{}", part_2);

    println!("Took {:?}ms", dur.as_millis());
}

fn set_of_three(edges: &HashMap<String, HashSet<String>>) -> u32 {
    let mut t_set: HashSet<Vec<String>> = HashSet::new();

    for (a, b_set) in edges.iter() {
        for b in b_set {
            let Some(c_set) = edges.get(b) else { continue };
            for c in c_set {
                if !b_set.contains(c) {
                    continue;
                }

                check_t_set(&mut t_set, a.clone(), b.clone(), c.clone());
            }
        }
    }

    t_set.len() as u32
}

fn check_t_set(t_set: &mut HashSet<Vec<String>>, a: String, b: String, c: String) {
    let mut v = [a, b, c];
    for u in v.iter() {
        if u.starts_with('t') {
            v.sort();
            t_set.insert(v.to_vec());
            return;
        }
    }
}

fn largest_set(edges: &HashMap<String, HashSet<String>>) -> String {
    let mut sets: HashSet<Vec<String>> = HashSet::new();
    let mut explored: HashSet<String> = HashSet::new();

    for (u, vs) in edges.iter() {
        if explored.contains(u) {
            continue;
        }

        explored.insert(u.clone());

        let mut us = vec![u.clone()];
        let vs = vs.iter().cloned().collect::<Vec<String>>();

        dfs(edges, &mut sets, &mut explored, &mut us, &vs);
    }

    let mut largest = Vec::new();
    for s in sets.iter() {
        if s.len() > largest.len() {
            largest = s.clone();
        }
    }

    largest.sort();
    largest.join(",")
}

fn dfs(
    edges: &HashMap<String, HashSet<String>>,
    sets: &mut HashSet<Vec<String>>,
    explored: &mut HashSet<String>,
    us: &mut Vec<String>,
    vs: &[String],
) {
    if vs.is_empty() {
        sets.insert(us.clone());
        return;
    }

    for v in vs.iter() {
        if explored.contains(v) {
            continue;
        }

        explored.insert(v.clone());

        let mut next_vs = Vec::new();
        for u in vs.iter() {
            if edges[u].contains(v) {
                next_vs.push(u.clone());
            }
        }

        us.push(v.clone());
        dfs(edges, sets, explored, us, &next_vs);
        us.pop();
    }
}
