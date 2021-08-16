/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "Config.h"
#include "DatabaseEnv.h"
#include "Log.h"
#include "ScriptMgr.h"
#include "StringFormat.h"

class IpTracker : public AccountScript, public WorldScript
{
public:
    IpTracker() : AccountScript("IpTracker"), WorldScript("IpTracker") { }

    void OnLastIpUpdate(uint32 accountId, std::string ip) override
    {
        if (!sConfigMgr->GetOption<bool>("IpTracker.Enabled", false))
        {
            return;
        }

        std::string query = Acore::StringFormatFmt("INSERT INTO `account_ip` (`account`, `ip`, `first_time`, `last_time`) VALUES ({}, '{}', NOW(), NOW()) ON DUPLICATE KEY UPDATE `last_time` = NOW()", accountId, ip);
        LoginDatabase.Execute(query.c_str());
    }

    void OnStartup() override
    {
        const auto DAYS = sConfigMgr->GetOption<int32>("IpTracker.CleanOlderThanDays", 0);

        if (!sConfigMgr->GetOption<bool>("IpTracker.Enabled", false) || DAYS == 0)
        {
            return;
        }

        LoginDatabase.PQuery("DELETE FROM `account_ip` WHERE `last_time` < (DATE_SUB(NOW(), INTERVAL %u DAY))", DAYS);

        FMT_LOG_INFO("modules", ">> IP Tracker: deleted all rows older than {} days", DAYS);
    }
};

void AddSC_IpTracker()
{
    new IpTracker();
}
