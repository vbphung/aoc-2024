use bit_set::BitSet;
use recur_topo::recur_topo;
use std::{
    collections::{HashMap, HashSet, VecDeque},
    io::BufRead,
};

mod recur_topo;

const N: usize = 100;

fn main() {
    let f = match std::fs::File::open("input.txt") {
        Ok(f) => f,
        Err(err) => panic!("Problem opening input file: {err:?}"),
    };

    let r = std::io::BufReader::new(f);
    let lns: Vec<String> = r.lines().map(|x| x.unwrap()).collect();

    let mut part_1 = 0;
    let mut part_2 = 0;
    let mut part_2_recur = 0;

    let mut dag = vec![HashSet::<usize>::new(); N];

    let mut i: usize = 0;
    while i < lns.len() && !lns[i].is_empty() {
        let spl: Vec<&str> = lns[i].split('|').collect();
        let u: usize = spl[0].parse().unwrap();
        let v: usize = spl[1].parse().unwrap();
        dag[u].insert(v);

        i += 1;
    }

    i += 1;

    while i < lns.len() {
        let order: Vec<usize> = lns[i].split(',').map(|x| x.parse().unwrap()).collect();
        if follows_rules(&dag, &order) {
            part_1 += order[order.len() / 2];
        } else {
            let ins = topo(&dag, &order);
            part_2 += ins[ins.len() / 2];

            let ins = recur_topo(&dag, &order, N);
            part_2_recur += ins[ins.len() / 2];
        }

        i += 1;
    }

    println!("{}", part_1);
    println!("{}", part_2);

    assert!(part_2_recur == part_2);
}

fn follows_rules(dag: &[HashSet<usize>], order: &[usize]) -> bool {
    for i in 1..order.len() {
        for j in 0..i {
            if dag[order[i]].contains(&order[j]) {
                return false;
            }
        }
    }
    true
}

fn topo(dag: &[HashSet<usize>], order: &[usize]) -> Vec<usize> {
    let mut ins = Vec::new();

    let order: BitSet = order.iter().cloned().collect();
    let mut graph: HashMap<usize, Vec<usize>> = HashMap::new();
    for (u, vs) in dag.iter().enumerate() {
        if !order.contains(u) {
            continue;
        }

        for &v in vs.iter() {
            if order.contains(v) {
                graph.entry(v).or_default().push(u);
            }
        }
    }

    let mut degree = HashMap::new();
    for &u in graph.keys() {
        degree.entry(u).or_insert(0);

        for &v in &graph[&u] {
            *degree.entry(v).or_insert(0) += 1;
        }
    }

    let mut q = VecDeque::new();
    for (&u, &d) in &degree {
        if d == 0 {
            q.push_back(u);
        }
    }

    while let Some(u) = q.pop_front() {
        ins.push(u);

        if let Some(vs) = graph.get(&u) {
            for &v in vs {
                *degree.get_mut(&v).unwrap() -= 1;
                if degree[&v] == 0 {
                    q.push_back(v);
                }
            }
        }
    }

    ins
}
