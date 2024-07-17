#include <dispatch/dispatch.h>
#include <inttypes.h>

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


typedef int* ktrace_session_t;


#define F_OPENFROM		56              /* SPI: open a file relative to fd (must be a dir) */
#define F_UNLINKFROM		57              /* SPI: open a file relative to fd (must be a dir) */
#define F_CHECK_OPENEVT		58              /* SPI: if a process is marked OPENEVT, or in O_EVTONLY on opens of this vnode */

#define F_MARKDEPENDENCY	60             /* this process hosts the device supporting the fs backing this fd */

#define F_SETSTATICCONTENT	68              /*
                                                         * indicate to the filesystem/storage driver that the content to be
                                                         * written is usually static.  a nonzero value enables it, 0 disables it.
                                                         */
#define F_MOVEDATAEXTENTS	69              /* Swap only the data associated with two files */

#define F_GETDEFAULTPROTLEVEL	79 /* Get the default protection level for the filesystem */
#define F_MAKECOMPRESSED	80 /* Make the file compressed; truncate & toggle BSD bits */
#define F_SET_GREEDY_MODE	81      /*
                                                 * indicate to the filesystem/storage driver that the content to be
                                                 * written should be written in greedy mode for additional speed at
                                                 * the cost of storage efficiency. A nonzero value enables it, 0 disables it.
                                                 */

#define F_SETIOTYPE		82  /*
                                     * Use parameters to describe content being written to the FD. See
                                     * flag definitions below for argument bits.
                                     */

#define F_RECYCLE		84      /* Recycle vnode; debug/development builds only */


#define F_OFD_SETLK             90      /* Acquire or release open file description lock */
#define F_OFD_SETLKW            91      /* (as F_OFD_SETLK but blocking if conflicting lock) */
#define F_OFD_GETLK             92      /* Examine OFD lock */

#define F_OFD_SETLKWTIMEOUT     93      /* (as F_OFD_SETLKW but return if timeout) */
#define F_OFD_GETLKPID          94      /* get record locking information */

#define F_SETCONFINED           95      /* "confine" OFD to process */
#define F_GETCONFINED           96      /* is-fd-confined? */


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

