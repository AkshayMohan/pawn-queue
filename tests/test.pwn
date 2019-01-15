//test.pwn | pawn-queue (queue.inc) testing code.

#include <a_samp>
#include <queue>

const TEST_QUEUE_SIZE = 4, MAX_ROWS = 2, MAX_COLS = 2;
new
	Queue:g_QTest1<TEST_QUEUE_SIZE>,
	QueueDT:Float:g_fQTest2<TEST_QUEUE_SIZE>,
	CircularQueue:g_cQTest3<TEST_QUEUE_SIZE>,//same as QueueC:
	QueueCDT:Float:g_cfQTest4<TEST_QUEUE_SIZE>;

//Multi-dimensional queues.
new
	QueueArray:qArray[MAX_ROWS]<TEST_QUEUE_SIZE>,
	QueueMatrix:qMatrix[MAX_ROWS][MAX_COLS]<TEST_QUEUE_SIZE>,
	
	QueueCArray:qcArray[MAX_ROWS]<TEST_QUEUE_SIZE>,
	QueueCMatrix:qcMatrix[MAX_ROWS][MAX_COLS]<TEST_QUEUE_SIZE>;

main() {

	new i, j, k, Float:fVar;
	for(i = 0; i< TEST_QUEUE_SIZE; i++) {

	    queue_enqueue(g_QTest1, random(random(10)));
	    queue_enqueue(g_cQTest3, random(10));

		fVar = float(random(10));
	    queue_enqueue(g_fQTest2, fVar);
	    
	    fVar = float(random(10));
	    queue_enqueue(g_cfQTest4, fVar);
	}
	printf("Linear Queue | Test 1 | Integer-type");
	queue_print(g_QTest1);
	
	printf("Linear Queue | Test 2 | Float-type");
	queue_print(g_fQTest2);
	
	printf("Circular Queue | Test 3 | Integer-type");
	queue_print(g_cQTest3);
	
	printf("Circular Queue | Test 4 | Float-type");
	queue_print(g_cfQTest4);
	
	printf("The last element of Test1 is : %d", g_QTest1[queue_rear(g_QTest1)]);
	printf("The first element of Test1 is : %d", g_QTest1[queue_front(g_QTest1)]);
	
	queue_dequeue(g_QTest1, i);
	printf("Deleted %d from Test1 (Linear)", i);
	
	queue_dequeue(g_cQTest3, i);
	printf("Deleted %d from Test3 (Circular)", i);
	
	if(queue_enqueue(g_QTest1, 2) == -1)
	    printf("Failed inserting 2 to Test1 (Linear)");
	else
	    printf("Successfully inserted 2 to Test1 (Linear)");
	    
	if(queue_enqueue(g_cQTest3, 2) == -1)
	    printf("Failed inserting 2 to Test3 (Circular)");
	else
	    printf("Successfully inserted 2 to Test3 (Circular)");
	    
	printf("Circular Queue | Test 3 | Integer-type");
	queue_print(g_cQTest3);
	
	printf("Forcefully inserting element : 100 to Test3 (Circular)");
	queue_force_insert(g_cQTest3, 100);

	printf("Circular Queue | Test 3 | Integer-type");
	queue_print(g_cQTest3);
	
	printf("Test1 (Linear) | Front = %d | Rear = %d", queue_front(g_QTest1),
	                                            queue_rear(g_QTest1));

	printf("Test3 (Circular) | Front = %d | Rear = %d", queue_front(g_cQTest3),
	                                            queue_rear(g_cQTest3));

	printf("Replacing 100 with 1000 in Test3 (Circular)");
	queue_replace(g_cQTest3, 100, 1000);

	printf("Circular Queue | Test 3 | Integer-type");
	queue_print(g_cQTest3);

	printf("Deleting element 1000 from Test3 (Circular) <Not dequeue operation>");
	queue_delete_element(g_cQTest3, 1000);
	
	printf("Circular Queue | Test 3 | Integer-type");
	queue_print(g_cQTest3);
	
	printf("Testing multi-dimensional queues...");
	
	//Queue matrix should always be initialized due to PAWN limitation.
	queue_init(qMatrix);
	queue_init(qcMatrix);

	//Adding some values to queue arrays and matrices.
	for(i = 0; i< MAX_ROWS; i++) {
	
	    for(k = 0; k< TEST_QUEUE_SIZE; k++) {
	    
		    queue_enqueue(qArray[i], random(10));
		    //printf("qArray | front=%d | rear=%d", queue_front(qArray[i]), queue_rear(qArray[i]));
		    queue_enqueue(qcArray[i], random(10));
		    //printf("qcArray | front = %d | rear = %d", queue_front(qcArray[i]), queue_rear(qcArray[i]));
		}
	    for(j = 0; j< MAX_COLS; j++) {
	    
	        for(k = 0; k< TEST_QUEUE_SIZE; k++) {
	        
	            //printf("[%d,%d,%d] | Front = %d | Rear = %d", i, j, k, queue_front(qMatrix[i][j]), queue_rear(qMatrix[i][j]));
		        queue_enqueue(qMatrix[i][j], random(10));
		        queue_enqueue(qcMatrix[i][j], random(10));
			}
		}
	}
	for(i = 0; i< MAX_ROWS; i++) {
	
	    printf("Displaying qArray[%d] (Linear) :", i);
	    queue_print(qArray[i]);
	    
	    printf("Displaying qcArray[%d] (Circular) :", i);
	    queue_print(qcArray[i]);
	}
	for(i = 0; i< MAX_ROWS; i++) {
	
	    for(j = 0; j< MAX_COLS; j++) {
	    
	        printf("Displaying qMatrix[%d][%d] (Linear)", i, j);
	        queue_print(qMatrix[i][j]);

	        printf("Displaying qcMatrix[%d][%d] (Circular)", i, j);
	        queue_print(qcMatrix[i][j]);
		}
	}
}

public OnGameModeInit() {

	printf("===================================");
	printf("    Queue Testing - ParK_iY");
	printf("===================================");
	SetGameModeText("Queue - Testing");
	return 1;
}
