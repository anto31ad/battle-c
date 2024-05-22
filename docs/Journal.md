# battle-c Journal

## Week 2024-05-21

### Displaying messages during a game session

I decided to re-engineer the message queue system.

The reason for using a queue is that, during a game session, the game state has to be displayed *after* it's been entirely updated, rather than displaying a message after every incremental update.

My initial approach of creating an enum of standardized messages (`MessageType`) was not optimal for several reasons

- if I were to add a new message, I would have to edit the code in many places
- It was becoming progressively difficult to follow the flow of calls 
- if the message required params, I would need to make those available by the time the message is to be generated; for instance, in order to display a message like "(x, y) have already been called", I would need to make x and y available for later consumption (by the `process_message()` function).

The alternative I am going for is a queue of strings along with a buffer I can write on.

### Other

- I mistakenly wrote lines like `val = counter++;` thinking they would be equivalent to `val = counter + 1`... Silly me!
