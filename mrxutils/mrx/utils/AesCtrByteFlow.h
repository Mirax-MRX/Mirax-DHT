
#pragma once

#include /utils/ByteFlow.h"
#include /utils/common.h"
#include /utils/crypto.h"
#include /utils/Slice.h"
#include /utils/Status.h"
#include /utils/UInt.h"

namespace td {

#if TD_HAVE_OPENSSL
class AesCtrByteFlow : public ByteFlowInplaceBase {
 public:
  void init(const UInt256 &key, const UInt128 &iv) {
    state_.init(as_slice(key), as_slice(iv));
  }
  void init(AesCtrState &&state) {
    state_ = std::move(state);
  }
  AesCtrState move_aes_ctr_state() {
    return std::move(state_);
  }
  void loop() override {
    bool was_updated = false;
    while (true) {
      auto ready = input_->prepare_read();
      if (ready.empty()) {
        break;
      }
      state_.encrypt(ready, MutableSlice(const_cast<char *>(ready.data()), ready.size()));
      input_->confirm_read(ready.size());
      output_.advance_end(ready.size());
      was_updated = true;
    }
    if (was_updated) {
      on_output_updated();
    }
    if (!is_input_active_) {
      finish(Status::OK());  // End of input stream.
    }
    set_need_size(1);
  }

 private:
  AesCtrState state_;
};
#endif

}  // namespace td
