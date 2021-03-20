
#include "Config.h"
#include "LoginDatabase.h"
#include "ScriptMgr.h"

#define CONF_ENABLE_TRACKER "IpTracker.Enabled"
#define CONF_CLEAN_OLDER_THAN_DAYS "IpTracker.CleanOlderThanDays"
#define QUERY_SELECT_IP "SELECT `ip` FROM `account_ip` WHERE `account` = %u AND `ip` = '%s'"
#define QUERY_INSERT_IP "INSERT IGNORE INTO `account_ip` (`account`, `ip`, `first_time`, `last_time`) VALUES (%u, '%s', NOW(), NOW());"
#define QUERY_UPDATE_IP "UPDATE `account_ip` SET `last_time` = NOW() WHERE `account` = %u AND `ip` = '%s'"
#define QUERY_DELETE_IP "DELETE FROM `account_ip` WHERE `last_time` < (DATE_SUB(NOW(), INTERVAL %u DAY))"

class IpTracker : public AccountScript, public WorldScript {
public:

    IpTracker() : AccountScript("IpTracker"), WorldScript("IpTracker") { }

    void OnLastIpUpdate(uint32 accountId, std::string ip) override
    {
        if (!sConfigMgr->GetOption<bool>(CONF_ENABLE_TRACKER, false))
        {
            return;
        }

        auto result = LoginDatabase.PQuery(QUERY_SELECT_IP, accountId, ip.c_str());

        if (!result || result->GetRowCount() == 0)
        {
            LoginDatabase.PQuery(QUERY_INSERT_IP, accountId, ip.c_str());
        }
        else
        {
            LoginDatabase.PQuery(QUERY_UPDATE_IP, accountId, ip.c_str());
        }
    }

    void OnStartup() override
    {
        const auto DAYS = sConfigMgr->GetOption<int32>(CONF_CLEAN_OLDER_THAN_DAYS, 0);

        if (!sConfigMgr->GetOption<bool>(CONF_ENABLE_TRACKER, false) || DAYS == 0)
        {
            return;
        }

        LoginDatabase.PQuery(QUERY_DELETE_IP, DAYS);
        sLog->outString(">>> mod-ip-tracker: deleted all rows older than %u days\n", DAYS);
    }
};

void AddIpTrackerScripts() {
    new IpTracker();
}

