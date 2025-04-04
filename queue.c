#include "queue.h"
#include "tile_game.h"

void enqueue(struct queue *q, struct game_state state) 
{
    uint64_t state_serial = serialize(state);
    insert_at_tail(&(q->data), state_serial);
}

struct game_state dequeue(struct queue *q) { 
    struct game_state state = deserialize(remove_from_head(&(q->data)));
    // printf("New Head: %p\n", q->data.head);
    return state; 
}

int check_solved(struct game_state s)
{
    // printf("Checking if solved\n");
    // if (s.empty_row != 4 || s.empty_col != 4)
    // {
    //     return 0; 
    // }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 1; j <= 4; j++)
        {
            if (s.tiles[i][j-1] != 4*i+j && !(i == 3 && j == 4 && s.tiles[i][j-1] == 0))
            {
                return 0; 
            }
        }
    }
    return 1; 
}

void print_state(struct game_state state)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j <= 3; j++)
        {
            printf("%d ", state.tiles[i][j]);
        }
        printf("\n");
    }
}

int number_of_moves(struct game_state start) 
{ 
    struct linked_list data = {.head=NULL};
    struct queue q = {.data=data};
    enqueue(&q, start);
    // printf("%d\n", check_solved(start));
    while (q.data.head != NULL)
    {
        // printf("Value: %ld\n", q.data.head->value);
        struct game_state current = dequeue(&q);
        // print_state(current);
        // printf("Entered while loop\n");
        if (check_solved(current) == 1)
        {
            // printf("Chekcing in while loop\n");
            free_list(q.data);
            return current.num_steps;
        }
        else
        {
            // Queue all possible moves
            // printf("Queueing while loop\n");
            struct game_state move_up_state = current; 
            struct game_state move_down_state = current; 
            struct game_state move_left_state = current; 
            struct game_state move_right_state = current; 
            move_up(&move_up_state);
            move_down(&move_down_state);
            move_right(&move_right_state);
            move_left(&move_left_state);
            if (move_up_state.num_steps != current.num_steps)
            {
                // printf("%p\n", q.data.head);
                enqueue(&q, move_up_state);
            }
            if (move_down_state.num_steps != current.num_steps)
            {
                enqueue(&q, move_down_state);
            }
            if (move_left_state.num_steps != current.num_steps)
            {
                enqueue(&q, move_left_state);
            }
            if (move_right_state.num_steps != current.num_steps)
            {
                enqueue(&q, move_right_state);
            }
        }
    }
    free_list(q.data);
    return 0; 
}
