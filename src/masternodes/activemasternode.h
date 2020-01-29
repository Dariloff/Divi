// Copyright (c) 2014-2016 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ACTIVEMASTERNODE_H
#define ACTIVEMASTERNODE_H

#include <init.h>
#include <key.h>
#include <masternodes/masternode.h>
#include <masternodes/masternodeconfig.h>
#include <net.h>
#include <sync.h>
#include <wallet/wallet.h>

#define ACTIVE_MASTERNODE_INITIAL 0 // initial state
#define ACTIVE_MASTERNODE_SYNC_IN_PROCESS 1
#define ACTIVE_MASTERNODE_INPUT_TOO_NEW 2
#define ACTIVE_MASTERNODE_NOT_CAPABLE 3
#define ACTIVE_MASTERNODE_STARTED 4

// Responsible for activating the Masternode and pinging the network
class CActiveMasternode
{
private:
    // critical section to protect the inner data structures
    mutable CCriticalSection cs;

    /// Ping Masternode
    bool SendMasternodePing(std::string& errorMessage, CConnman &connman);

    static bool GetVinFromOutput(CWallet &wallet, COutput out, CTxIn& vin, CPubKey& pubkey, CKey& secretKey);
    /// Register any Masternode
    static bool Register(CMasternodeBroadcast &mnb, CConnman &connman,bool deferRelay = false);

    /// Get 10000 PIV input that can be used for the Masternode
    static bool GetMasterNodeVin(CWallet &wallet, CTxIn& vin, CPubKey& pubkey, CKey& secretKey, std::string strTxHash, std::string strOutputIndex);

public:
    // Initialized by init.cpp
    // Keys for the main Masternode
    CPubKey pubKeyMasternode;

    // Initialized while registering Masternode
    CTxIn vin;
    CService service;

    int status;
    std::string notCapableReason;

    CActiveMasternode()
    {
        status = ACTIVE_MASTERNODE_INITIAL;
    }

    /// Manage status of main Masternode
    void ManageStatus(CWallet &wallet, CConnman &connman);
    std::string GetStatus();

    /// Register remote Masternode
    static bool Register(
        CWallet &wallet, 
        const CMasternodeConfig::CMasternodeEntry& config, 
        std::string& errorMessage,
        CConnman &connman,
        bool deferRelay,
        CMasternodeBroadcast& mnb);

    /// Get 10000 PIV input that can be used for the Masternode
    bool GetMasterNodeVin(CWallet &wallet, CTxIn& vin, CPubKey& pubkey, CKey& secretKey);
    static vector<COutput> SelectCoinsMasternode(CWallet &wallet);

    /// Enable cold wallet mode (run a Masternode with no funds)
    bool EnableHotColdMasterNode(CTxIn& vin, CService& addr);
};

extern CActiveMasternode activeMasternode;
// TODO fix - remove these extern variables appropriately
extern bool fMasterNode;
extern std::string strMasterNodePrivKey;
extern std::string strMasterNodeAddr;

#endif
