/*

							queue.inc
			Implementation of Queue data structure in PAWN.

@Author 	:	ParK_iY AKA "Akshay Mohan"
@Version 	:   v1.2.0
@Github     :   https://github.com/AkshayMohan/queue.inc

Last Update :   15th of January, 2019.

MIT License

Copyright (c) 2019 Akshay Mohan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#if defined _included_queue_
	#endinput
#endif

#define _included_queue_
/*

Library functions:

native queue_init(QueueMatrix[][]); //Only 2D queues require.
native queue_enqueue(Queue[], value);
native queue_insert(Queue[], value);
native queue_add(Queue[], value);
native queue_force_insert(Queue[], value); //Overwrites if full.
native queue_force_add(Queue[], value); //Overwrites if full.
native queue_force_enqueue(Queue[], value); //Overwrites if full.
native queue_dequeue(Queue[], &value); //Stores the deleted item in "value".
native queue_delete(Queue[], &value); //Stores the deleted item in "value".
native queue_remove(Queue[], &value); //Stores the deleted item in "value".
native queue_delete_element(Queue[], value); //Deletes the item from queue.
native queue_is_empty(Queue[]);
native queue_is_full(Queue[]);
native queue_search(Queue[], value);
native queue_replace(Queue[], old_value, new_value);
native queue_print(Queue[]); //For debugging purposes.
native queue_front(Queue[]);
native queue_rear(Queue[]);
native queue_reset(Queue[]);
native queue_clear(Queue[]);
native queue_size(Queue[]);
native queue_to_array(Queue[], arr[]);
native array_to_queue(Queue[], arr[], size = sizeof(arr));

//Functions are documented below.
*/


#if !defined _STREAMER_TAGS_
	#define _STREAMER_TAGS_ \
				DynamicObject, \
				DynamicPickup, \
				DynamicCP, \
				DynamicRaceCP, \
				DynamicMapIcon, \
				DynamicText3D, \
				DynamicArea, \
				DynamicActor
#endif

#if !defined _STRONG_TAGS_
	#if defined _CUSTOM_TAGS_
		#define _STRONG_TAGS_ \
		    _, \
		    Float, \
		    Menu, \
		    Text, \
		    Text3D, \
			/* Streamer tags */ \
			_STREAMER_TAGS, \
			_CUSTOM_TAGS_
	#else
	    #define _STRONG_TAGS_ \
	        _, \
			Float, \
	        Menu, \
	        Text, \
	        Text3D, \
			/* Streamer tags */ \
			_STREAMER_TAGS_
	#endif
#endif

#define 				CircularQueue 			QueueC
#define 				CircularQueueDT 		QueueCDT
#define 				CircularQueueArray      QueueCArray
#define                 CircularQueueDTArray    QueueCDTArray
#define                 CircularQueueMatrix     QueueCMatrix
#define                 CircularQueueDTMatrix   QueueCDTMatrix

#define QueueC:%0<%1> \
	%0[%1], QueueC:front_%0 = QueueC:-1, QueueC:rear_%0 = QueueC:-1, QSZ_%0 = %1

#define QueueCArray:%0[%2]<%1> \
	%0[%2][%1], QueueC:front_%0[%2] = {QueueC:-1, ...}, \
	QueueC:rear_%0[%2] = {QueueC:-1, ...}, QSZ_%0[%2] = {%1, ...}

#define QueueCMatrix:%0[%2][%3]<%1> \
	%0[%2][%3][%1], QueueC:front_%0[%2][%3], QueueC:rear_%0[%2][%3], \
	QSZ_%0[%2][%3]

#define QueueCDT:%2:%0<%1> \
	%2:%0[%1], QueueC:front_%0 = QueueC:-1, QueueC:rear_%0 = QueueC:-1, \
	QSZ_%0 = %1

#define QueueCDTArray:%2:%0[%3]<%1> \
	%2:%0[%3][%1], QueueC:front_%0[%3] = {QueueC:-1, ...}, \
	QueueC:rear_%0[%3] = {QueueC:-1, ...}, QSZ_%0[%3] = {%1, ...}

#define QueueCDTMatrix:%2:%0[%3][%4]<%1> \
	%2:%0[%3][%4][%1], QueueC:front_%0[%3][%4], QueueC:rear_%0[%3][%4], \
	QSZ_%0[%3][%4]

#define Queue:%0<%1> \
	%0[%1], front_%0 = -1, rear_%0 = -1, QSZ_%0 = %1

#define QueueArray:%0[%2]<%1> \
	%0[%2][%1], front_%0[%2] = {-1, ...}, rear_%0[%2] = {-1, ...}, \
	QSZ_%0[%2] = {%1, ...}

#define QueueMatrix:%0[%2][%3]<%1> \
	%0[%2][%3][%1], front_%0[%2][%3], rear_%0[%2][%3], QSZ_%0[%2][%3]

#define QueueDT:%2:%0<%1> \
	%2:%0[%1], front_%0 = -1, rear_%0 = -1, QSZ_%0 = %1

#define QueueDTArray:%2:%0[%3]<%1> \
	%2:%0[%3][%1], front_%0[%3] = {-1, ...}, rear_%0[%3] = {-1, ...}, \
	QSZ_%0[%3] = {%1, ...}

#define QueueDTMatrix:%2:%0[%3][%4]<%1> \
	%2:%0[%3][%4][%1], front_%0[%3][%4], rear_%0[%3][%4], QSZ_%0[%3][%4]


/*
#TODO : Both queue-array and queue-matrix should share common size variable.
		Functions should be common for Queue, QueueArray and QueueMatrix.
*/

/**
	<function>
	    queue_init({QueueMatrix, QueueCMatrix, QueueDTMatrix, QueueCDTMatrix}:
	                QueueMatrix[][]);
	</function>

	<summary>
	    Initializes 2-D queue (queue matrix).
	</summary>

	<param name="QueueMatrix[][]">
	    Matrix queue to be initialized.
	</param>

	<returns>
	    This function always returns 1.
	</returns>
*/
#define queue_init(%0) \
	internal_queue_init(front_%0, rear_%0, QSZ_%0, sizeof(front_%0), \
 	sizeof(front_%0[]), sizeof(%0[][]))

/**
	<function>
	    queue_is_full({Queue, QueueC, QueueDT, QueueCDT}:Queue[]);
	</function>

	<summary>
	    Checks if the given Queue is full.
	</summary>

	<param name="Queue[]">
	    Queue to be checked on.
	</param>

	<returns>
		1 if the queue is full. 0 otherwise.
	</returns>
*/
#define queue_is_full(%0) \
	internal_is_queue_full(front_%0, rear_%0, QSZ_%0)

/**
	<function>
	    queue_is_empty({Queue, QueueC, QueueDT, QueueCDT}:Queue[]);
	</function>

	<summary>
	    Checks if the given Queue is empty.
	</summary>

	<param name="Queue[]">
	    Queue to be checked on.
	</param>

	<returns>
	    1 if the queue is empty. 0 otherwise.
	</returns>
*/
#define queue_is_empty(%0) \
	internal_is_queue_empty(rear_%0)

/**
	<function>
	    queue_insert({Queue, QueueC, QueueDT, QueueCDT}:Queue[], value);
	</function>

	<summary>
	    Inserts given element to the queue.
	</summary>

	<param name="Queue[]">
	    Queue on which value is to be inserted.
	</param>

	<param name="value">
		The value to be inserted.
	</param>

	<returns>
	    -1 if the function fails. Otherwise the rear most position of the queue.
	</returns>
*/
#define queue_insert(%0,%1) \
	internal_queue_insert(%0, %1, front_%0, rear_%0, QSZ_%0)

/**
	<function>
	    queue_force_insert({Queue, QueueC, QueueDT, QueueCDT}:Queue[], value);
	</function>

	<summary>
	    Forcefully inserts given element to the queue. Overwrites the rear most
	    position if queue is already full.
	</summary>

	<param name="Queue[]">
	    Queue on which value is to be inserted.
	</param>

	<param name="value">
		The value to be inserted.
	</param>

	<returns>
	    The rear most position of the queue.
	</returns>
*/
#define queue_force_insert(%0,%1) \
	internal_queue_force_insert(%0, %1, front_%0, rear_%0, QSZ_%0)

/**
	<function>
	    queue_delete({Queue, QueueC, QueueDT, QueueCDT}:Queue[], &value);
	</function>

	<summary>
	    Deletes the first element from the queue and stores it in second arg.
	</summary>

	<param name="Queue[]">
	    Queue on which delete operation is to be performed.
	</param>

	<param name="&value">
	    Holds the value of the deleted element.
	</param>

	<returns>
	    -1 if the function fails. Otherwise the rear most position of the queue.
	</returns>
*/
#define queue_delete(%0,%1) \
	internal_queue_delete(%0, front_%0, rear_%0, %1, QSZ_%0)

/**
	<function>
	    queue_search({Queue, QueueC, QueueDT, QueueCDT}:Queue[], value);
	</function>

	<summary>
	    Searches the given queue for a particular value.
	</summary>

	<param name="Queue[]">
	    Queue to be searched on.
	</param>

	<param name="value">
	    Value to be searched for from the queue.
	</param>

	<returns>
	    -1 if the element is not found. Otherwise the subscript on which element
	    is found.
	</returns>
*/
#define queue_search(%0,%1) \
	internal_queue_search(%0, %1, front_%0, rear_%0, QSZ_%0)

/**
	<function>
		queue_replace({Queue, QueueC, QueueDT, QueueCDT}:Queue[], old_value,
		                new_value);
	</function>

	<summary>
	    Replaces old value with new value on the given queue.
	</summary>

	<param name="Queue[]">
	    Queue on which replace operation has to be performed.
	</param>

	<param name="old_value">
	    Old value to be replaced.
	</param>

	<param name="new_value">
	    New value to which the function replaces the old value.
	</param>

	<returns>
		-1 if old_value is not found. Otherwise the subscript of the replaced
		data.
	</returns>
*/
#define queue_replace(%0,%1,%2) \
	internal_queue_replace(%0, %1, %2, front_%0, rear_%0, QSZ_%0)

/**
	<function>
	    queue_to_array({Queue, QueueC, QueueDT, QueueCDT}:Queue[], arr[]);
	</function>

	<summary>
		Copies the contents of a queue to an array.
	</summary>

	<param name="Queue[]">
	    Queue which has to be copied.
	</param>

	<param name="arr[]">
	    Array on which data is copied to.
	</param>

	<returns>
		Function always returns 1.
	</returns>
*/
#define queue_to_array(%0,%1) \
	internal_queue_to_arr(%0, front_%0, rear_%0, %1, QSZ_%0)

/**
	<function>
	    array_to_queue({Queue, QueueC, QueueDT, QueueCDT}:Queue[], arr[],
						size = sizeof(arr))
	</function>

	<summary>
		Builds a queue from given array.
	</summary>

	<param name="Queue[]">
	    Queue to build.
	</param>

	<param name="arr[]">
		Array from which queue has to build.
	</param>

	<param name="size">
		Size of the array. (Optional)
	</param>

	<returns>
	    Function always returns 1.
	</returns>
*/
#define array_to_queue(%0,%1) \
	internal_arr_to_queue(%0, front_%0, rear_%0, %1, QSZ_%0)
/**
	<function>
	    queue_delete_element({Queue, QueueC, QueueDT, QueueCDT}:Queue[], value);
	</function>

	<summary>
	    Deletes a particular element from queue without breaking FIFO rule.
	</summary>

	<param name="Queue[]">
	    Queue from which value has to be deleted.
	</param>

	<param name="value">
	    Value to be deleted.
	</param>

	<returns>
		-1 if value does not exist. Otherwise the subscript of removed element.
	</returns>
*/
#define queue_delete_element(%0,%1) \
	internal_queue_del_element(%0, %1, front_%0, rear_%0, QSZ_%0)

/**
	<function>
	    queue_print({Queue, QueueC, QueueDT, QueueCDT}:Queue[]);
	</function>

	<summary>
	    Prints the given queue to SA-MP console. Meant only for debugging.
	</summary>

	<param name="Queue[]">
	    Queue to be printed.
	</param>

	<returns>
		Function always returns 1.
	</returns>
*/
#define queue_print(%0) \
	internal_print_queue(%0, front_%0, rear_%0, QSZ_%0)

/**
	<function>
	    queue_rear({Queue, QueueC, QueueDT, QueueCDT}:Queue[]);
	</function>

	<summary>
	    Returns the rear most position of the given queue.
	</summary>

	<param name="Queue[]">
	    Queue from which rear most position is returned.
	</param>

	<returns>
	    The rear most position of the queue.
	</returns>
*/
#define queue_rear(%0) \
	_:rear_%0

/**
	<function>
	    queue_front({Queue, QueueC, QueueDT, QueueCDT}:Queue[]);
	</function>

	<summary>
	    Returns the front most position of the given queue.
	</summary>

	<param name="Queue[]">
	    Queue from which front most position is returned.
	</param>

	<returns>
	    The front most position of the queue.
	</returns>
*/
#define queue_front(%0) \
	_:front_%0

/**
	<function>
	    queue_size({Queue, QueueC, QueueDT, QueueCDT}:Queue[]);
	</function>

	<summary>
	    Returns the size of the given queue.
	</summary>

	<param name="Queue[]">
	    Queue from which it's size is returned.
	</param>

	<returns>
	    Size of the queue.
	</returns>
*/
#define queue_size(%0) \
	QSZ_%0

/**
	<function>
	    queue_reset({Queue, QueueC, QueueDT, QueueCDT}:Queue[]);
	</function>

	<summary>
	    Resets the given queue. (Empties it)
	</summary>

	<param name="Queue[]">
	    Queue which has to be reset.
	</param>

	<returns>
		Function does not return anything in particular.
	</returns>
*/
#define queue_reset(%0) \
	front_%0 = rear_%0 = -1

/**
	<function>
	    queue_add({Queue, QueueC, QueueDT, QueueCDT}:Queue[], value);
	</function>

	<summary>
	    Inserts given element to the queue.
	</summary>

	<param name="Queue[]">
	    Queue on which value is to be inserted.
	</param>

	<param name="value">
		The value to be inserted.
	</param>

	<returns>
	    -1 if the function fails. Otherwise the rear most position of the queue.
	</returns>
*/
#define queue_add 			queue_insert

/**
	<function>
	    queue_enqueue({Queue, QueueC, QueueDT, QueueCDT}:Queue[], value);
	</function>

	<summary>
	    Inserts given element to the queue.
	</summary>

	<param name="Queue[]">
	    Queue on which value is to be inserted.
	</param>

	<param name="value">
		The value to be inserted.
	</param>

	<returns>
	    -1 if the function fails. Otherwise the rear most position of the queue.
	</returns>
*/
#define queue_enqueue       queue_insert

/**
	<function>
	    queue_force_add({Queue, QueueC, QueueDT, QueueCDT}:Queue[], value);
	</function>

	<summary>
	    Forcefully inserts given element to the queue. Overwrites the rear most
	    position if queue is already full.
	</summary>

	<param name="Queue[]">
	    Queue on which value is to be inserted.
	</param>

	<param name="value">
		The value to be inserted.
	</param>

	<returns>
	    The rear most position of the queue.
	</returns>
*/
#define queue_force_add     queue_force_insert

/**
	<function>
	    queue_force_enqueue({Queue, QueueC, QueueDT, QueueCDT}:Queue[], value);
	</function>

	<summary>
	    Forcefully inserts given element to the queue. Overwrites the rear most
	    position if queue is already full.
	</summary>

	<param name="Queue[]">
	    Queue on which value is to be inserted.
	</param>

	<param name="value">
		The value to be inserted.
	</param>

	<returns>
	    The rear most position of the queue.
	</returns>
*/
#define queue_force_enqueue 	queue_force_insert

/**
	<function>
	    queue_remove({Queue, QueueC, QueueDT, QueueCDT}:Queue[], &value);
	</function>

	<summary>
	    Deletes the first element from the queue and stores it in second arg.
	</summary>

	<param name="Queue[]">
	    Queue on which delete operation is to be performed.
	</param>

	<param name="&value">
	    Holds the value of the deleted element.
	</param>

	<returns>
	    -1 if the function fails. Otherwise the rear most position of the queue.
	</returns>
*/
#define queue_remove    queue_delete

/**
	<function>
	    queue_dequeue({Queue, QueueC, QueueDT, QueueCDT}:Queue[], &value);
	</function>

	<summary>
	    Deletes the first element from the queue and stores it in second arg.
	</summary>

	<param name="Queue[]">
	    Queue on which delete operation is to be performed.
	</param>

	<param name="&value">
	    Holds the value of the deleted element.
	</param>

	<returns>
	    -1 if the function fails. Otherwise the rear most position of the queue.
	</returns>
*/
#define queue_dequeue   queue_delete

/**
	<function>
	    queue_clear({Queue, QueueC, QueueDT, QueueCDT}:Queue[]);
	</function>

	<summary>
	    Resets the given queue. (Empties it)
	</summary>

	<param name="Queue[]">
	    Queue which has to be reset.
	</param>

	<returns>
		Function does not return anything in particular.
	</returns>
*/
#define queue_clear     queue_reset


//Internal functions.
stock internal_is_queue_full({_, QueueC}:queueFront, {_, QueueC}:queueRear, size,
							tagid = tagof(queueFront)) {

	new upperBound = size - 1;
	if(tagid == tagof(QueueC:)) {

	    if((queueRear == upperBound && queueFront == 0) ||
	        queueRear == queueFront - 1)
	        return 1;
		return 0;
	}
	else if(queueRear == upperBound)
	    return 1;
	return 0;
}

stock internal_is_queue_empty({_, QueueC}:queueFront) {

	if(queueFront == -1)
	    return 1;
	return 0;
}

stock internal_queue_insert({_STRONG_TAGS_}:Queue[],
							{_STRONG_TAGS_}:value,
							&{_, QueueC}:queueFront, &{_, QueueC}:queueRear,
							size,
							tagid = tagof(queueFront)) {

	if(tagid == tagof(QueueC:)) {

		if(internal_is_queue_full(QueueC:queueFront, QueueC:queueRear, size))
		    return -1;
	} else {

	    if(internal_is_queue_full(queueFront, queueRear, size))
	        return -1;
	}
	if(queueFront == -1)
	    queueFront = queueRear = 0;
	else if(tagid == tagof(QueueC:) && (queueRear == size - 1))
		queueRear = 0;
	else
	    queueRear++;

	Queue[queueRear] = value;
	return queueRear;
}

stock internal_queue_force_insert({_STRONG_TAGS_}:Queue[],
									{_STRONG_TAGS_}:value,
									&QueueC:queueFront, &QueueC:queueRear,
									size) {

	if(queueFront == QueueC:-1)
	    queueFront = queueRear = QueueC:0;
	else if(queueRear == QueueC:(size - 1)) {

	    if(!queueFront)
	        queueFront = QueueC:1;
	    queueRear = QueueC:0;
	}
	else if(queueFront == queueRear + QueueC:1) {

	    if(queueFront == QueueC:(size - 1))
	        queueFront = QueueC:0;
		else
		    queueFront++;

		queueRear++;
	}
	Queue[_:queueRear] = value;
	return _:queueRear;
}

stock internal_queue_delete({_STRONG_TAGS_}:Queue[],
							&{_, QueueC}:queueFront, &{_, QueueC}:queueRear,
							&{_STRONG_TAGS_}:value,
							size,
							tagid = tagof(queueFront)) {

	if(internal_is_queue_empty(queueFront))
	    return -1;
	value = Queue[queueFront];
	if(queueFront == queueRear)
	    queueFront = queueRear = -1;
	else if(tagid == tagof(QueueC:) && queueFront == size - 1)
	    queueFront = 0;
	else
	    queueFront++;
	return queueFront;
}

stock internal_queue_search({_STRONG_TAGS_}:Queue[], {_STRONG_TAGS_}:value,
							{_, QueueC}:queueFront, {_, QueueC}:queueRear,
							size,
							tagid = tagof(queueFront)) {

	new i;
	if(tagid != tagof(QueueC:) || queueRear >= queueFront) {

	    for(i = queueFront; i <= queueRear; i++) {

	        if(Queue[i] == value)
				return i;
		}
	}
	else {

	    for(i = queueFront; i< size; i++) {

			if(Queue[i] == value)
			    return i;
		}
		for(i = 0; i <= queueRear; i++) {

		    if(Queue[i] == value)
		        return i;
		}
	}
	return -1;
}

stock internal_queue_replace({_STRONG_TAGS_}:Queue[], {_STRONG_TAGS_}:value1,
							{_STRONG_TAGS_}:value2,
							{_, QueueC}:queueFront, {_, QueueC}:queueRear,
							size,
							tagid = tagof(queueFront)) {

	new idx;
	if(tagid == tagof(QueueC:))
	    idx = internal_queue_search(Queue, value1, QueueC:queueFront, QueueC:queueRear,
									size);
	else
	    idx = internal_queue_search(Queue, value1, queueFront, queueRear, size);
	if(idx != -1)
	    Queue[idx] = value2;
	return idx;
}

stock internal_queue_to_arr({_STRONG_TAGS_}:Queue[],
							{_, QueueC}:queueFront, {_, QueueC}:queueRear,
							{_STRONG_TAGS_}:arr[],
							size, tagid = tagof(queueFront)) {

	new i, j = 0;
	if(tagid != tagof(QueueC:) || queueRear >= queueFront) {

	    for(i = queueFront; i <= queueRear; i++)
	        arr[j++] = Queue[i];
	 } else {

	    for(i = queueFront; i < size; i++)
	        arr[j++] = Queue[i];

		for(i = 0; i <= queueRear; i++)
		    arr[j++] = Queue[i];
	}
	return 1;
}

stock internal_arr_to_queue({_STRONG_TAGS_}:Queue[],
							&{_, QueueC}:queueFront, &{_, QueueC}:queueRear,
							{_STRONG_TAGS_}:arr[],
							&queue_size,
							size = sizeof(arr)) {
	new i;
	for(i = 0, queueFront = 0, queueRear = size - 1,
		queue_size = size; i< size; i++)
		Queue[i] = arr[i];
	return 1;
}

stock internal_queue_del_element({_STRONG_TAGS_}:Queue[], {_STRONG_TAGS_}:value,
							&{_, QueueC}:queueFront, &{_, QueueC}:queueRear,
							size,
							tagid = tagof(queueFront)) {

	new idx, i;
	if(tagid == tagof(QueueC:)) {

	    idx = internal_queue_search(Queue, value, QueueC:queueFront, QueueC:queueRear,
									size);
	    if(idx != -1) {

			if(queueRear >= queueFront) {

			    for(i = idx + 1; i <= queueRear; i++)
			        Queue[i-1] = Queue[i];
			}
			else {

				for(i = idx + 1; i < size; i++)
				    Queue[i-1] = Queue[i];

				Queue[i-1] = Queue[0];
				for(i = 1; i <= queueRear; i++)
				    Queue[i-1] = Queue[i];
			}
			queueRear--;
		}
	} else {

	    idx = internal_queue_search(Queue, value, queueFront, queueRear, size);
	    if(idx != -1) {

	        for(i = idx + 1; i <= queueRear; i++)
	            Queue[i - 1] = Queue[i];
			queueRear--;
		}
	}
	return idx;
}

//Only for 2D queues (QueueMatrix)
stock internal_queue_init({_, QueueC}:queueFront[][], {_, QueueC}:queueRear[][],
							size[][], rows, cols, size_) {

	new i, j;
	for(i = 0; i< rows; i++) {

	    for(j = 0; j< cols; j++) {

	    	queueFront[i][j] = queueRear[i][j] = -1;
	    	size[i][j] 	= size_;
		}
	}
	return 1;
}

//For debugging purposes.
stock internal_print_queue({_STRONG_TAGS_}:Queue[],
							{_, QueueC}:queueFront, {_, QueueC}:queueRear,
							size,
							tagid = tagof(queueFront),
							qtagid = tagof(Queue)) {

	new i, sformatSpecifier[3];
	if(qtagid == tagof(Float:))
	    sformatSpecifier = "%f";
	else
	    sformatSpecifier = "%d";
	if(tagid != tagof(QueueC:) || queueRear >= queueFront) {

	    for(i = queueFront; i <= queueRear; i++)
	        printf(sformatSpecifier, Queue[i]);
	} else {

	    for(i = queueFront; i < size; i++)
	        printf(sformatSpecifier, Queue[i]);

		for(i = 0; i <= queueRear; i++)
		    printf(sformatSpecifier, Queue[i]);
	}
	return 1;
}
