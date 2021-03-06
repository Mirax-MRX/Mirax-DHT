
#pragma once

#include "dht-remote-node.hpp"
#include "dht.hpp"

#include <map>

namespace ton {

namespace dht {

class DhtMember;

class DhtBucket {
 private:
  double ping_timeout_ = 60;
  td::uint32 max_missed_pings_ = 3;

  std::vector<std::unique_ptr<DhtRemoteNode>> active_nodes_;
  std::vector<std::unique_ptr<DhtRemoteNode>> backup_nodes_;

  //std::map<td::UInt256, std::unique_ptr<DhtRemoteNode>> pending_nodes_;
  td::uint32 k_;
  //bool check_one(td::actor::ActorId<adnl::Adnl> adnl, td::actor::ActorId<DhtMember> node, adnl::AdnlNodeIdShort src,
  //               const DhtMember::PrintId &print_id);
  void demote_node(size_t idx);
  void promote_node(size_t idx);

 public:
  DhtBucket(td::uint32 k) : k_(k) {
    active_nodes_.resize(k);
    backup_nodes_.resize(k);
  }
  td::uint32 active_cnt();
  td::Status add_full_node(DhtKeyId id, DhtNode node, td::actor::ActorId<adnl::Adnl> adnl,
                           adnl::AdnlNodeIdShort self_id);
  void check(bool client_only, td::actor::ActorId<adnl::Adnl> adnl, td::actor::ActorId<DhtMember> node,
             adnl::AdnlNodeIdShort src);
  void receive_ping(DhtKeyId id, DhtNode result, td::actor::ActorId<adnl::Adnl> adnl, adnl::AdnlNodeIdShort self_id);
  void get_nearest_nodes(DhtKeyId id, td::uint32 bit, DhtNodesList &vec, td::uint32 k);
  void dump(td::StringBuilder &sb) const;
  DhtNodesList export_nodes() const;
};

}  // namespace dht

}  // namespace ton
