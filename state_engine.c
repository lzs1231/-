#include <stdlib.h>
#include <stdio.h>

#define transition_num  11
#define state_num 6

typedef void (*action_foo)() ;

enum state { s_stop='s', s_play='p', s_forward='f', s_backward='b', s_pause='_', s_record='r'  };
enum message { play, stop, forward, backward, record, pause };

/* action starts */
void do_stop() {printf ("I am in state stop and should doing something here.\n");}
void do_play() {printf ("I am in state play and should doing something here.\n");}
void do_forward() {printf ("I am in state forward and should doing something here.\n");}
void do_backward() {printf ("I am in state backward and should doing something here.\n");}
void do_pause() {printf ("I am in state pause and should doing something here.\n");}
void do_record() {printf ("I am in state record and should doing something here.\n");}
struct state_action {
    enum state m_state;
    action_foo foo;
};
struct state_action state_action_map[state_num] = {
    {s_stop, do_stop},
    {s_play, do_play},
    {s_forward, do_forward},
    {s_backward, do_backward},
    {s_pause, do_pause},
    {s_record, do_record}};

action_foo ACTION_NOT_FOUND = NULL;
action_foo lookup_action(enum state s, struct state_action* a)
{
    action_foo ret = ACTION_NOT_FOUND;
    int i=0;
    for (i=0;i<state_num;++i)
    {
        if(s == a[i].m_state)
        {
            ret = a[i].foo;
        }
    }
    return ret;
}
/*  action ends. */

/* transition starts*/
struct transition {
    enum state current;
    enum message m;
    enum state next;
};

struct transition fsm[transition_num] = {
        /* current_state, message/event, next_state*/
    {s_play,	stop,		s_stop},
    {s_play,	pause,		s_pause},
    {s_pause,	pause,		s_play},
    {s_pause,	stop,		s_stop},
    {s_stop,	forward,	s_forward},
    {s_stop,	play,		s_play},
    {s_stop,	backward,	s_backward},
    {s_stop,	record,		s_record},
    {s_forward,	stop,		s_stop},
    {s_backward,	stop,		s_stop},
    {s_record,	stop,		s_stop} };

int const ERR = -1;
int lookup_transition (enum state s, enum message m, struct transition * t)
{
    int ret=ERR;
    int i;
    for(i=0;i<transition_num;++i)
    {
        if(t[i].current == s && t[i].m == m)
        {
            ret = i;
        }
    }
    return ret;
}
/* transition ends*/

void state_change(enum message m)
{
    static state = s_stop;
    enum state next;
    int index = 0;

    index = lookup_transition(state, m, fsm);
    if(index!=ERR)
    {
        state = fsm[index].next;
        lookup_action(state, state_action_map)();
    }
    return;
}


int main(int argc, char *argv[])
{
    char c=0x00;
    while(1)
    {
        c = getchar();
        printf ("%c input.\n", c);
        switch(c)
        {
            case ' ': state_change(pause); break;
            case 'p':  state_change(play); break;
            case 'r': state_change(record); break;
            case 's': state_change(stop); break;
            case 'f': state_change(forward); break;
            case 'b': state_change(backward); break;
            case 'q': return EXIT_SUCCESS;
        }

        
    }
    
    return EXIT_SUCCESS;
}
