import threading
import queue
import os
from typing import Callable, Iterable, Optional

def thread_batched_map[X, Y](fn: Callable[[X], Y], iterable: Iterable[X], chunk_size: int = 16, worker_count: Optional[int] = None) -> Iterable[Y]:
    """
    Applies `fn` to items from `iterable` in parallel using threads, processing items in batches.
    Args:
        fn: Function to apply to each item.
        iterable: Iterable of items to process.
        chunk_size: Number of items per chunk.
        worker_count: Number of worker threads (defaults to os.cpu_count()).
    Yields:
        Results in the order they are processed (not input order).
    """
    if worker_count is None:
        worker_count = os.cpu_count() or 4

    q = queue.Queue(maxsize=worker_count * 2)
    results_q = queue.Queue()

    def producer():
        batch = []
        for item in iterable:
            batch.append(item)
            if len(batch) == chunk_size:
                q.put(batch)
                batch = []
        if batch:
            q.put(batch)
        for _ in range(worker_count):
            q.put(None)

    def worker():
        while True:
            batch = q.get()
            if batch is None:
                break
            results = [fn(x) for x in batch]
            for r in results:
                results_q.put(r)
            q.task_done()

    prod_thread = threading.Thread(target=producer)
    prod_thread.daemon = True
    prod_thread.start()

    workers = []
    for _ in range(worker_count):
        t = threading.Thread(target=worker)
        t.daemon = True
        t.start()
        workers.append(t)

    finished = False
    outstanding = 0
    try:
        while True:
            if not finished:
                prod_thread.join(timeout=0.01)
                if not prod_thread.is_alive():
                    finished = True
            try:
                result = results_q.get(timeout=0.05)
                yield result
            except queue.Empty:
                if finished and all(not t.is_alive() for t in workers) and results_q.empty():
                    break
    finally:
        # Clean up: ensure all threads exit
        for _ in range(worker_count):
            q.put(None)
        for t in workers:
            t.join(timeout=0.25)
