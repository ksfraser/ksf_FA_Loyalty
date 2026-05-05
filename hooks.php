<?php
/**
 * FA_Loyalty Module Hooks for FrontAccounting
 * Customer Loyalty Program (depends on ksf_FA_CRM)
 */

define('SS_LOYALTY', 118 << 8);

class hooks_fa_loyalty extends hooks {
    var $module_name = 'fa_loyalty';

    function install_options($app) {
        global $path_to_root;

        switch($app->id) {
            case 'CRM':
                $app->add_rapp_function(0, _("Loyalty Program"),
                    $path_to_root."/modules/".$this->module_name."/loyalty.php", 'SA_LOYALTYVIEW', MENU_ENTRY);
                break;
            case 'Sales':
                $app->add_lapp_function(0, _("Customer Loyalty"),
                    $path_to_root."/modules/".$this->module_name."/loyalty.php", 'SA_LOYALTYVIEW', MENU_ENTRY);
                break;
        }
    }

    function install_access() {
        $security_sections[SS_LOYALTY] = _("Loyalty Program");
        $security_areas['SA_LOYALTYVIEW'] = array(SS_LOYALTY | 1, _("View Loyalty"));
        $security_areas['SA_LOYALTYCREATE'] = array(SS_LOYALTY | 2, _("Manage Loyalty"));
        return array($security_areas, $security_sections);
    }

    function activate_extension($company, $check_only=true) {
        $updates = array(
            'sql/fa_customer_loyalty.sql' => array($this->module_name),
            'sql/fa_loyalty_transactions.sql' => array($this->module_name)
        );
        return $this->update_databases($company, $updates, $check_only);
    }

    function db_prevoid($trans_type, $trans_no) {
        // Handle voiding if loyalty module tracks point transactions
    }
}
?>
