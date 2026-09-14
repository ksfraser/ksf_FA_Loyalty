<?php
/**
 * DB functions for fa_loyalty_transactions table
 */

function write_loyalty_transaction($transaction_id, $data) {
    if ($transaction_id) {
        $sql = "UPDATE `" . TB_PREF . "fa_loyalty_transactions` SET ";
        $sets = array();
        foreach (array('loyalty_id', 'transaction_type', 'points', 'reference_type', 'reference_id', 'description') as $field) {
            if (isset($data[$field])) {
                $sets[] = "`$field` = " . db_escape($data[$field]);
            }
        }
        $sql .= implode(', ', $sets) . " WHERE `transaction_id` = " . db_escape($transaction_id);
    } else {
        $sql = "INSERT INTO `" . TB_PREF . "fa_loyalty_transactions` SET ";
        $sets = array();
        foreach (array('loyalty_id', 'transaction_type', 'points', 'reference_type', 'reference_id', 'description') as $field) {
            if (isset($data[$field])) {
                $sets[] = "`$field` = " . db_escape($data[$field]);
            }
        }
        $sql .= implode(', ', $sets);
    }
    
    db_query($sql, "Failed to write loyalty transaction");
    return $transaction_id ? $transaction_id : db_insert_id();
}

function get_loyalty_transactions($loyalty_id, $limit = 50) {
    $sql = "SELECT * FROM `" . TB_PREF . "fa_loyalty_transactions` 
        WHERE `loyalty_id` = " . db_escape($loyalty_id) . "
        ORDER BY `created_at` DESC
        LIMIT " . (int)$limit;
    return db_query($sql, "Failed to get loyalty transactions");
}

function get_transaction($transaction_id) {
    $sql = "SELECT * FROM `" . TB_PREF . "fa_loyalty_transactions` 
        WHERE `transaction_id` = " . db_escape($transaction_id);
    $result = db_query($sql, "Failed to get transaction");
    return db_fetch($result);
}

function delete_loyalty_transaction($transaction_id) {
    $sql = "DELETE FROM `" . TB_PREF . "fa_loyalty_transactions` 
        WHERE `transaction_id` = " . db_escape($transaction_id);
    db_query($sql, "Failed to delete transaction");
}
?>
