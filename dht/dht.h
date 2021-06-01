
#pragma once

#include "mrx/utils/int_types.h"

#include "mrx/actor/PromiseFuture.h"

#include "mrx/actor/actor.h"

#include "adnl/adnl.h"

#include "auto/tl/mirax_api.hpp"

#include "dht-types.h"

namespace ton {

namespace dht {

class DhtGlobalConfig;

class Dht : public td::actor::Actor {
 public:
  static td::Result<td::actor::ActorOwn<Dht>> create(adnl::AdnlNodeIdShort id, std::string db_root,
                                                     std::shared_ptr<DhtGlobalConfig> conf,
                                                     td::actor::ActorId<keyring::Keyring> keyring,
                                                     td::actor::ActorId<adnl::Adnl> adnl);
  static td::Result<td::actor::ActorOwn<Dht>> create_client(adnl::AdnlNodeIdShort id, std::string db_root,
                                                            std::shared_ptr<DhtGlobalConfig> conf,
                                                            td::actor::ActorId<keyring::Keyring> keyring,
                                                            td::actor::ActorId<adnl::Adnl> adnl);
  static td::Result<std::shared_ptr<DhtGlobalConfig>> create_global_config(
      tl_object_ptr<ton_api::dht_config_global> conf);

  virtual adnl::AdnlNodeIdShort get_id() const = 0;

  virtual void set_value(DhtValue key_value, td::Promise<td::Unit> result) = 0;
  virtual void get_value(DhtKey key, td::Promise<DhtValue> result) = 0;

  virtual void dump(td::StringBuilder &sb) const = 0;

  virtual ~Dht() = default;
};

}  // namespace dht

}  // namespace ton
