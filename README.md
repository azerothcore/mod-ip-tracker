# mod-ip-tracker

## Description

Storing all IPs from all accounts and their login times in `account_ip`.


## Requirements

mod-ip-tracker requires:

- AzerothCore v3.0.0+


## Installation

```
1) Simply `git clone` the module under the `modules` directory of your AzerothCore source or copy paste it manually.
2) Import the `auth/base/account_ip.sql` SQL file manually to the `acore_auth` database
3) Re-run cmake and launch a clean build of AzerothCore
4) Configure `ip-tracker.conf`
```
