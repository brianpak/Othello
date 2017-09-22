# Othello

### SimpleAITurn()</br>
The simple AI player will make a move randomly from currently available moves.

### GoodAITurn()</br>
The good AI player will make the best move possible, calculated using the minimax algorithm.

### With lookahead depth = 7, the good AI beats the simple AI 95% of the time.

---

### Serial GoodAITurn()
With the serial version, the good AI only uses one main thread.

### Parallel GoodAITurn()
With the parallel version, the good AI increases spawning & syncing as the lookahead depth gets deeper.

### With lookahead depth = 7, the parallel code runs 17 times faster than the serial code.

---

