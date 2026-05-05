<?php
/**
 * DB functions for fa_customer_loyalty table
 */

function write_customer_loyalty($loyalty_id, $data) {
    if ($loyalty_id) {
        $sql = "UPDATE `" . TB_PREF . "fa_customer_loyalty` SET ";
        $sets = array();
        foreach (array('customer_person_id', 'points_balance', 'tier_level', 'enrollment_date') as $field) {
            if (isset($data[$field])) {
                $sets[] = "`$field` = " . db_escape($data[$field]);
            }
        }
        $sql .= implode(', ', $sets) . " WHERE `loyalty_id` = " . db_escape($loyalty_id);
    } else {
        $sql = "INSERT INTO `" . TB_PREF . "fa_customer_loyalty` SET ";
        $sets = array();
        foreach (array('customer_person_id', 'points_balance', 'tier_level', 'enrollment_date') as $field) {
            if (isset($data[$field])) {
                $sets[] = "`$field` = " . db_escape($data[$field]);
            }
        }
        $sql .= implode(', ', $sets);
    }
    
    db_query($sql, "Failed to write customer loyalty");
    return $loyalty_id ? $loyalty_id : db_insert_id();
}

function get_customer_loyalty($loyalty_id) {
    $sql = "SELECT l.*, p.name as customer_name, p.email
        FROM `" . TB_PREF . "fa_customer_loyalty` l
        LEFT JOIN `" . TB_PREF . "crm_persons` p ON l.customer_person_id = p.id
        WHERE l.`loyalty_id` = " . db_escape($loyalty_id);
    $result = db_query($sql, "Failed to get loyalty record");
    return db_fetch($result);
}

function get_loyalty_by_customer($person_id) {
    $sql = "SELECT * FROM `" . TB_PREF . "fa_customer_loyalty` 
        WHERE `customer_person_id` = " . db_escape($person_id);
    $result = db_query($sql, "Failed to get loyalty by customer");
    return db_fetch($result);
}

function update_loyalty_points($loyalty_id, $points_change) {
    $sql = "UPDATE `" . TB_PREF . "fa_customer_loyalty` 
        SET `points_balance` = `points_balance` + " . db_escape($points_change) . "
        WHERE `loyalty_id` = " . db_escape($loyalty_id);
    db_query($sql, "Failed to update loyalty points");
}

function delete_customer_loyalty($loyalty_id) {
    $sql = "DELETE FROM `" . TB_PREF . "fa_customer_loyalty` WHERE `loyalty_id` = " . db_escape($loyalty_id);
    db_query($sql, "Failed to delete loyalty record");
}
?>
