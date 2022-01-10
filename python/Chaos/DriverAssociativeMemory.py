#!/opt/local/bin/ python
# -*- coding: utf-8 -*-

import AssociativeMemory
import matplotlib.pyplot as plt

test = AssociativeMemory.AssociativeMemory(1000)
recall =  test.recall(0.1)

plt.plot(range(len(recall)), recall)
plt.show()

