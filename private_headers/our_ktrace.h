// NOTE: all of these are reverse engineered

#include <dispatch/dispatch.h>
#include <inttypes.h>

typedef int* ktrace_session_t;

typedef struct {
	uint64_t	timestamp;
	uintptr_t	arg1;
	uintptr_t	arg2;
	uintptr_t	arg3;
	uintptr_t	arg4;
	uint64_t	threadid;
	uint32_t	debugid;
	uint32_t	unk;
	struct timeval	walltime;
} *ktrace_event_t;

/*struct kd_record {
	int32_t  cpu;
	uint32_t debugid;
	int64_t timestamp;
	kd_buf_argtype arg1;
	kd_buf_argtype arg2;
	kd_buf_argtype arg3;
	kd_buf_argtype arg4;
	kd_buf_argtype arg5;
} __attribute__((packed));

typedef struct kd_record *ktrace_event_t;*/


// ----- ktrace private stuff -----
// NOTE: These are best guesses
#define KTRACE_FEATURE_DISABLED 0
#define KTRACE_FEATURE_LAZY 2

ktrace_session_t ktrace_session_create(void);
int ktrace_session_destroy(ktrace_session_t s);

pid_t ktrace_get_pid_for_thread(ktrace_session_t s, int threadid);
const char *ktrace_get_execname_for_thread(ktrace_session_t s, int threadid);

char * ktrace_get_path_for_vp(ktrace_session_t s, int vnodeid);

int ktrace_filter_pid(ktrace_session_t s, pid_t pid);
int ktrace_exclude_pid(ktrace_session_t s, pid_t pid);
int ktrace_exclude_process(ktrace_session_t s, char *procname); 
int ktrace_filter_process(ktrace_session_t s, char *procname);

int ktrace_start(ktrace_session_t s, dispatch_queue_main_t queue);
int ktrace_end(ktrace_session_t s, int unk1);

int ktrace_set_execnames_enabled(ktrace_session_t s, int unk);
int ktrace_set_vnode_paths_enabled(ktrace_session_t s, int unk);
int ktrace_set_uuid_map_enabled(ktrace_session_t s, int unk);

int ktrace_convert_timestamp_to_nanoseconds(ktrace_session_t s, uint64_t mach, uint64_t *nanoseconds);

void ktrace_set_signal_handler(ktrace_session_t s);

int ktrace_set_file(ktrace_session_t s, char *name);

int ktrace_ignore_process_filter_for_event(ktrace_session_t s, int event);

void ktrace_session_set_default_event_names_enabled(ktrace_session_t s, int);

// TODO: figure out the block definitions!!!
void ktrace_set_completion_handler(ktrace_session_t s, void *block);
void ktrace_set_dropped_events_handler(ktrace_session_t s, void *block);
void ktrace_events_range(ktrace_session_t s, int start, int end, void *block);

