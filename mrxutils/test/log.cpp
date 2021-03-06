

#include /utils/benchmark.h"
#include /utils/FileLog.h"
#include /utils/format.h"
#include /utils/logging.h"
#include /utils/port/path.h"
#include /utils/port/thread.h"
#include /utils/Slice.h"
#include /utils/tests.h"
#include /utils/TsFileLog.h"

#include <functional>
#include <limits>

char disable_linker_warning_about_empty_file_tdutils_test_log_cpp TD_UNUSED;

#if !TD_THREAD_UNSUPPORTED
template <class Log>
class LogBenchmark : public td::Benchmark {
 public:
  LogBenchmark(std::string name, int threads_n, std::function<td::unique_ptr<Log>()> creator)
      : name_(std::move(name)), threads_n_(threads_n), creator_(std::move(creator)) {
  }
  std::string get_description() const override {
    return PSTRING() << name_ << " " << td::tag("threads_n", threads_n_);
  }
  void start_up() override {
    log_ = creator_();
    threads_.resize(threads_n_);
  }
  void tear_down() override {
    for (auto path : log_->get_file_paths()) {
      td::unlink(path).ignore();
    }
    log_.reset();
  }
  void run(int n) override {
    for (auto &thread : threads_) {
      thread = td::thread([this, n] { this->run_thread(n); });
    }
    for (auto &thread : threads_) {
      thread.join();
    }
  }

  void run_thread(int n) {
    auto str = PSTRING() << "#" << n << " : fsjklfdjsklfjdsklfjdksl\n";
    for (int i = 0; i < n; i++) {
      if (i % 10000 == 0) {
        log_->rotate();
      }
      log_->append(str);
    }
  }

 private:
  std::string name_;
  td::unique_ptr<Log> log_;
  int threads_n_{0};
  std::function<td::unique_ptr<Log>()> creator_;
  std::vector<td::thread> threads_;
};

template <class F>
static void bench_log(std::string name, int threads_n, F &&f) {
  bench(LogBenchmark<typename decltype(f())::element_type>(std::move(name), threads_n, std::move(f)));
};

TEST(Log, TsLogger) {
  bench_log("NewTsFileLog", 4,
            [] { return td::TsFileLog::create("tmplog", std::numeric_limits<td::int64>::max(), false).move_as_ok(); });
  bench_log("TsFileLog", 8, [] {
    class FileLog : public td::LogInterface {
     public:
      FileLog() {
        file_log_.init("tmplog", std::numeric_limits<td::int64>::max(), false).ensure();
        ts_log_.init(&file_log_);
      }
      ~FileLog() {
      }
      void append(td::CSlice slice) override {
        ts_log_.append(slice, -1);
      }
      std::vector<std::string> get_file_paths() override {
        return file_log_.get_file_paths();
      }

     private:
      td::FileLog file_log_;
      td::TsLog ts_log_{nullptr};
    };
    return td::make_unique<FileLog>();
  });

  bench_log("noop", 4, [] {
    class NoopLog : public td::LogInterface {
     public:
      void append(td::CSlice slice) override {
      }
    };
    return td::make_unique<NoopLog>();
  });

  bench_log("FileLog", 4, [] {
    class FileLog : public td::LogInterface {
     public:
      FileLog() {
        file_log_.init("tmplog", std::numeric_limits<td::int64>::max(), false).ensure();
      }
      ~FileLog() {
      }
      void append(td::CSlice slice) override {
        file_log_.append(slice, -1);
      }
      std::vector<std::string> get_file_paths() override {
        return file_log_.get_file_paths();
      }

     private:
      td::FileLog file_log_;
    };
    return td::make_unique<FileLog>();
  });
}
#endif
