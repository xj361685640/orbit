// Copyright (c) 2020 The Orbit Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ORBIT_LINUX_TRACING_TRACEPOINTS_H_
#define ORBIT_LINUX_TRACING_TRACEPOINTS_H_

#include <cstdint>

// Format is based on the content of the event's format file:
// /sys/kernel/debug/tracing/events/<category>/<name>/format

struct __attribute__((__packed__)) tracepoint_common {
  uint16_t common_type;
  uint8_t common_flags;
  uint8_t common_preempt_count;
  int32_t common_pid;
};

struct __attribute__((__packed__)) task_newtask_tracepoint {
  tracepoint_common common;
  int32_t pid;
  char comm[16];
  uint64_t clone_flags;
  int16_t oom_score_adj;
};

struct __attribute__((__packed__)) task_rename_tracepoint {
  tracepoint_common common;
  int32_t pid;
  char oldcomm[16];
  char newcomm[16];
  int16_t oom_score_adj;
};

struct __attribute__((__packed__)) amdgpu_cs_ioctl_tracepoint {
  tracepoint_common common;
  uint64_t sched_job_id;
  int32_t timeline;
  uint32_t context;
  uint32_t seqno;
  uint64_t dma_fence;  // This is an address.
  uint64_t ring_name;  // This is an address.
  uint32_t num_ibs;
};

struct __attribute__((__packed__)) amdgpu_sched_run_job_tracepoint {
  tracepoint_common common;
  uint64_t sched_job_id;
  int32_t timeline;
  uint32_t context;
  uint32_t seqno;
  uint64_t ring_name;  // This is an address.
  uint32_t num_ibs;
};

struct __attribute__((__packed__)) dma_fence_signaled_tracepoint {
  tracepoint_common common;
  int32_t driver;
  int32_t timeline;
  uint32_t context;
  uint32_t seqno;
};

#endif  // ORBIT_LINUX_TRACING_TRACEPOINTS_H_
