// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SHELL_APP_ELECTRON_CRASH_REPORTER_CLIENT_H_
#define SHELL_APP_ELECTRON_CRASH_REPORTER_CLIENT_H_

#if !defined(OS_WIN)

#include <memory>

#include "base/compiler_specific.h"
#include "base/macros.h"
#include "base/no_destructor.h"
#include "build/build_config.h"
#include "components/crash/core/app/crash_reporter_client.h"

class ElectronCrashReporterClient : public crash_reporter::CrashReporterClient {
 public:
  static void Create();

  static ElectronCrashReporterClient* Get();
  void SetCollectStatsConsent(bool upload_allowed);
  void SetUploadUrl(const std::string& url);
  void SetShouldRateLimit(bool rate_limit);
  void SetShouldCompressUploads(bool compress_uploads);
  void SetGlobalAnnotations(
      const std::map<std::string, std::string>& annotations);

  // crash_reporter::CrashReporterClient implementation.
#if !defined(OS_MACOSX)
  void SetCrashReporterClientIdFromGUID(
      const std::string& client_guid) override;
#endif

#if defined(OS_POSIX) && !defined(OS_MACOSX)
  void GetProductNameAndVersion(const char** product_name,
                                const char** version) override;
  void GetProductNameAndVersion(std::string* product_name,
                                std::string* version,
                                std::string* channel) override;
  base::FilePath GetReporterLogFilename() override;
#endif

  bool GetCrashDumpLocation(base::FilePath* crash_dir) override;

#if defined(OS_MACOSX) || defined(OS_LINUX)
  bool GetCrashMetricsLocation(base::FilePath* metrics_dir) override;
#endif

  bool IsRunningUnattended() override;

  bool GetCollectStatsConsent() override;

  bool GetShouldRateLimit() override;
  bool GetShouldCompressUploads() override;

  void GetProcessSimpleAnnotations(
      std::map<std::string, std::string>* annotations) override;

#if defined(OS_MACOSX)
  bool ReportingIsEnforcedByPolicy(bool* breakpad_enabled) override;
#endif

#if defined(OS_MACOSX) || defined(OS_LINUX)
  bool ShouldMonitorCrashHandlerExpensively() override;
#endif

  bool EnableBreakpadForProcess(const std::string& process_type) override;

#if defined(OS_MACOSX)
  void GetUploadUrl(std::string* url) override;
#endif

 private:
  friend class base::NoDestructor<ElectronCrashReporterClient>;

  std::string upload_url_;
  bool collect_stats_consent_;
  bool rate_limit_ = false;
  bool compress_uploads_ = false;
  std::map<std::string, std::string> global_annotations_;

  ElectronCrashReporterClient();
  ~ElectronCrashReporterClient() override;

  DISALLOW_COPY_AND_ASSIGN(ElectronCrashReporterClient);
};

#endif  // OS_WIN

#endif  // SHELL_APP_ELECTRON_CRASH_REPORTER_CLIENT_H_
