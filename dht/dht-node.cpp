
#include "dht-node.hpp"
#include "keys/encryptor.h"

namespace ton {

namespace dht {

td::Status DhtNode::update(tl_object_ptr<ton_api::dht_node> obj) {
  if (version_ && obj->version_ <= version_) {
    return td::Status::Error(ErrorCode::notready, "too old version");
  }
  auto signature = std::move(obj->signature_);
  auto B = serialize_tl_object(obj, true);

  TRY_RESULT(pub, adnl::AdnlNodeIdFull::create(obj->id_));
  TRY_RESULT(addr_list, adnl::AdnlAddressList::create(std::move(obj->addr_list_)));

  if (!addr_list.public_only()) {
    return td::Status::Error(ErrorCode::notready, "dht node must have only public addresses");
  }
  if (!addr_list.size()) {
    return td::Status::Error(ErrorCode::notready, "dht node must have >0 addresses");
  }

  TRY_RESULT(E, pub.pubkey().create_encryptor());
  TRY_STATUS(E->check_signature(B.as_slice(), signature.as_slice()));

  id_ = pub;
  addr_list_ = addr_list;
  version_ = obj->version_;
  signature_ = td::SharedSlice(signature.as_slice());

  return td::Status::OK();
}

tl_object_ptr<ton_api::dht_nodes> DhtNodesList::tl() const {
  std::vector<tl_object_ptr<ton_api::dht_node>> L;
  for (auto &n : list_) {
    L.emplace_back(n.tl());
  }
  return create_tl_object<ton_api::dht_nodes>(std::move(L));
}

}  // namespace dht

}  // namespace ton
