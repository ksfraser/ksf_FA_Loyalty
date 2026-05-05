<?php
/**
 * DB functions for fa_coupons table
 */

function write_coupon($coupon_id, $data) {
    if ($coupon_id) {
        $sql = "UPDATE `" . TB_PREF . "fa_coupons` SET ";
        $sets = array();
        foreach (array('coupon_code', 'discount_type', 'discount_value', 'valid_from', 'valid_to', 'max_uses', 'used_count', 'is_active') as $field) {
            if (isset($data[$field])) {
                $sets[] = "`$field` = " . db_escape($data[$field]);
            }
        }
        $sql .= implode(', ', $sets) . " WHERE `coupon_id` = " . db_escape($coupon_id);
    } else {
        $sql = "INSERT INTO `" . TB_PREF . "fa_coupons` SET ";
        $sets = array();
        foreach (array('coupon_code', 'discount_type', 'discount_value', 'valid_from', 'valid_to', 'max_uses', 'used_count', 'is_active') as $field) {
            if (isset($data[$field])) {
                $sets[] = "`$field` = " . db_escape($data[$field]);
            }
        }
        $sql .= implode(', ', $sets);
    }
    
    db_query($sql, "Failed to write coupon");
    return $coupon_id ? $coupon_id : db_insert_id();
}

function get_coupon($coupon_id) {
    $sql = "SELECT * FROM `" . TB_PREF . "fa_coupons` WHERE `coupon_id` = " . db_escape($coupon_id);
    $result = db_query($sql, "Failed to get coupon");
    return db_fetch($result);
}

function get_coupon_by_code($coupon_code) {
    $sql = "SELECT * FROM `" . TB_PREF . "fa_coupons` 
        WHERE `coupon_code` = " . db_escape($coupon_code) . "
        AND `is_active` = 1
        AND (`valid_from` IS NULL OR `valid_from` <= CURDATE())
        AND (`valid_to` IS NULL OR `valid_to` >= CURDATE())";
    $result = db_query($sql, "Failed to get coupon by code");
    return db_fetch($result);
}

function increment_coupon_usage($coupon_id) {
    $sql = "UPDATE `" . TB_PREF . "fa_coupons` 
        SET `used_count` = `used_count` + 1
        WHERE `coupon_id` = " . db_escape($coupon_id);
    db_query($sql, "Failed to increment coupon usage");
}

function get_all_coupons($active_only = true) {
    $sql = "SELECT * FROM `" . TB_PREF . "fa_coupons`";
    if ($active_only) {
        $sql .= " WHERE `is_active` = 1";
    }
    $sql .= " ORDER BY `coupon_code`";
    return db_query($sql, "Failed to get coupons");
}

function delete_coupon($coupon_id) {
    $sql = "UPDATE `" . TB_PREF . "fa_coupons` SET `is_active` = 0 WHERE `coupon_id` = " . db_escape($coupon_id);
    db_query($sql, "Failed to delete coupon");
}
?>
