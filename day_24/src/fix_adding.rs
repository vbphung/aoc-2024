use crate::Operation;
use std::collections::HashMap;

pub fn verify_z(ops: &HashMap<String, Operation>, wire: String, lv: u32) -> bool {
    println!("verify_z {} {}", wire, lv);

    if let Some(op) = ops.get(&wire) {
        return op.op == "XOR"
            && (sorted_pair(op.a.clone(), op.b.clone()) == make_wire_pair(lv)
                || (verify_intermediate_xor(ops, op.a.clone(), lv)
                    && verify_carry_bit(ops, op.b.clone(), lv))
                || (verify_intermediate_xor(ops, op.b.clone(), lv)
                    && verify_carry_bit(ops, op.a.clone(), lv)));
    }

    false
}

fn verify_intermediate_xor(ops: &HashMap<String, Operation>, wire: String, lv: u32) -> bool {
    println!("verify_intermediate_xor {} {}", wire, lv);

    if let Some(op) = ops.get(&wire) {
        return op.op == "XOR" && sorted_pair(op.a.clone(), op.b.clone()) == make_wire_pair(lv);
    }

    false
}

fn verify_carry_bit(ops: &HashMap<String, Operation>, wire: String, lv: u32) -> bool {
    println!("verify_carry_bit {} {}", wire, lv);

    if let Some(op) = ops.get(&wire) {
        return (lv == 1
            && op.op == "AND"
            && sorted_pair(op.a.clone(), op.b.clone()) == make_wire_pair(0))
            || (op.op == "OR"
                && ((verify_direct_carry(ops, op.a.clone(), lv - 1)
                    && verify_recarry(ops, op.b.clone(), lv - 1))
                    || (verify_direct_carry(ops, op.b.clone(), lv - 1)
                        && verify_recarry(ops, op.a.clone(), lv - 1))));
    }

    false
}

fn verify_direct_carry(ops: &HashMap<String, Operation>, wire: String, lv: u32) -> bool {
    println!("verify_direct_carry {} {}", wire, lv);

    if let Some(op) = ops.get(&wire) {
        return op.op == "AND" && sorted_pair(op.a.clone(), op.b.clone()) == make_wire_pair(lv);
    }

    false
}

fn verify_recarry(ops: &HashMap<String, Operation>, wire: String, lv: u32) -> bool {
    println!("verify_recarry {} {}", wire, lv);

    if let Some(op) = ops.get(&wire) {
        return op.op == "AND"
            && ((verify_intermediate_xor(ops, op.a.clone(), lv)
                && verify_carry_bit(ops, op.b.clone(), lv))
                || (verify_intermediate_xor(ops, op.b.clone(), lv)
                    && verify_carry_bit(ops, op.a.clone(), lv)));
    }

    false
}

fn make_wire_pair(lv: u32) -> Vec<String> {
    vec![format!("x{:02}", lv), format!("y{:02}", lv)]
}

fn sorted_pair(x: String, y: String) -> Vec<String> {
    let mut v = vec![x, y];
    v.sort();
    v
}
