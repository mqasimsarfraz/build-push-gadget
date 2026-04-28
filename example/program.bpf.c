// SPDX-License-Identifier: GPL-2.0

#include <vmlinux.h>
#include <bpf/bpf_helpers.h>

#include <gadget/buffer.h>
#include <gadget/macros.h>
#include <gadget/types.h>

struct event {
	gadget_timestamp timestamp_raw;
	gadget_comm comm[TASK_COMM_LEN];
	gadget_pid pid;
};

GADGET_TRACER_MAP(events, 1024 * 256);

GADGET_TRACER(exec, events, event);

SEC("tracepoint/syscalls/sys_enter_execve")
int hello(struct trace_event_raw_sys_enter *ctx)
{
	struct event *event;

	event = gadget_reserve_buf(&events, sizeof(*event));
	if (!event)
		return 0;

	event->timestamp_raw = bpf_ktime_get_boot_ns();
	event->pid = bpf_get_current_pid_tgid() >> 32;
	bpf_get_current_comm(&event->comm, sizeof(event->comm));

	gadget_submit_buf(ctx, &events, event, sizeof(*event));

	return 0;
}

char LICENSE[] SEC("license") = "GPL";
