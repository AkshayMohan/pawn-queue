/*

							queue.inc
			Implementation of Queue data structure in PAWN.

@Author 	:	ParK_iY AKA "Akshay Mohan"
@Version 	:   v1.0
@Github     :   https://github.com/AkshayMohan/queue.inc

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

native queue_enqueue(Queue[], value);
native queue_insert(Queue[], value);
native queue_add(Queue[], value);
native queue_force_insert(Queue[], value); //Overwrites if full.
native queue_force_add(Queue[], value); //Overwrites if full.
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


enum E_QUEUE_DATA {

		E_FRONT,
		E_REAR,
		E_SIZE
}

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

#define CircularQueue QueueC
#define CircularQueueDT QueueCDT

#define QueueC:%0<%1> \
	%0[%1], QueueC:Q_%0[E_QUEUE_DATA] = {-1, -1, %1}

#define QueueCDT:%2:%0<%1> \
	%2:%0[%1], QueueC:Q_%0[E_QUEUE_DATA] = {-1, -1, %1}

#define Queue:%0<%1> \
	%0[%1], Q_%0[E_QUEUE_DATA] = {-1, -1, %1}

#define QueueDT:%2:%0<%1> \
	%2:%0[%1], Q_%0[E_QUEUE_DATA] = {-1, -1, %1}

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
	internal_is_queue_full(Q_%0)

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
	internal_is_queue_empty(Q_%0)

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
	internal_queue_insert(%0, %1, Q_%0)

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
	internal_queue_force_insert(%0, %1, Q_%0)

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
	internal_queue_delete(%0, Q_%0, %1)

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
	internal_queue_search(%0, %1, Q_%0)

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
	internal_queue_replace(%0, %1, %2, Q_%0)

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
	internal_queue_to_arr(%0, Q_%0, %1)

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
	internal_arr_to_queue(%0, Q_%0, %1)
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
	internal_queue_del_element(%0, %1, Q_%0)

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
	internal_print_queue(%0, Q_%0)

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
	Q_%0[E_REAR]

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
	Q_%0[E_FRONT]

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
	Q_%0[E_SIZE]

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
	Q_%0[E_FRONT] = Q_%0[E_REAR] = -1

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
stock internal_is_queue_full({_, QueueC}:Queue[E_QUEUE_DATA],
							tagid = tagof(Queue)) {

	new i;
	#pragma unused i
	//For some reasons, compiler crashes when I begin with this if statement.

	if(tagid == tagof(QueueC:)) {

	    if((Queue[E_REAR] == Queue[E_SIZE] - 1 && Queue[E_FRONT] == 0) ||
	        Queue[E_REAR] == Queue[E_FRONT] - 1)
	        return 1;
		return 0;
	}
	else if(Queue[E_REAR] == Queue[E_SIZE] - 1)
	    return 1;
	return 0;
}

stock internal_is_queue_empty({_, QueueC}:Queue[E_QUEUE_DATA]) {

	if(Queue[E_FRONT] == -1)
	    return 1;
	return 0;
}

stock internal_queue_insert({_STRONG_TAGS_}:Queue[],
							{_STRONG_TAGS_}:value,
							{_, QueueC}:E_Queue[E_QUEUE_DATA],
							tagid = tagof(E_Queue)) {

	if(tagid == tagof(QueueC:)) {

		if(internal_is_queue_full(QueueC:E_Queue))
		    return -1;
	} else {

	    if(internal_is_queue_full(E_Queue))
	        return -1;
	}
	if(E_Queue[E_FRONT] == -1)
	    E_Queue[E_FRONT] = E_Queue[E_REAR] = 0;
	else if(tagid == tagof(QueueC:) && (E_Queue[E_REAR] == E_Queue[E_SIZE] - 1))
		E_Queue[E_REAR] = 0;
	else
	    E_Queue[E_REAR]++;

	Queue[E_Queue[E_REAR]] = value;
	return E_Queue[E_REAR];
}

stock internal_queue_force_insert({_STRONG_TAGS_}:Queue[],
									{_STRONG_TAGS_}:value,
									QueueC:E_Queue[E_QUEUE_DATA]) {

	if(E_Queue[E_FRONT] == -1)
	    E_Queue[E_FRONT] = E_Queue[E_REAR] = 0;
	else if(E_Queue[E_REAR] == E_Queue[E_SIZE] - 1) {

	    if(!E_Queue[E_FRONT])
	        E_Queue[E_FRONT] = 1;
	    E_Queue[E_REAR] = 0;
	}
	else if(E_Queue[E_FRONT] == E_Queue[E_REAR] + 1) {

	    if(E_Queue[E_FRONT] == E_Queue[E_SIZE] - 1)
	        E_Queue[E_FRONT] = 0;
		else
		    E_Queue[E_FRONT]++;

		E_Queue[E_REAR]++;
	}
	Queue[E_Queue[E_REAR]] = value;
	return E_Queue[E_REAR];
}

stock internal_queue_delete({_STRONG_TAGS_}:Queue[],
							{_, QueueC}:E_Queue[E_QUEUE_DATA],
							&{_STRONG_TAGS_}:value,
							tagid = tagof(E_Queue)) {

	if(internal_is_queue_empty(E_Queue))
	    return -1;
	value = Queue[E_Queue[E_FRONT]];
	if(E_Queue[E_FRONT] == E_Queue[E_REAR])
	    E_Queue[E_FRONT] = E_Queue[E_REAR] = -1;
	else if(tagid == tagof(QueueC:) && E_Queue[E_FRONT] == E_Queue[E_SIZE] - 1)
	    E_Queue[E_FRONT] = 0;
	else
	    E_Queue[E_FRONT]++;
	return E_Queue[E_FRONT];
}

stock internal_queue_search({_STRONG_TAGS_}:Queue[], {_STRONG_TAGS_}:value,
							{_, QueueC}:E_Queue[E_QUEUE_DATA],
							tagid = tagof(E_Queue)) {

	new i;
	if(tagid != tagof(QueueC:) || E_Queue[E_REAR] >= E_Queue[E_FRONT]) {

	    for(i = E_Queue[E_FRONT]; i <= E_Queue[E_REAR]; i++) {

	        if(Queue[i] == value)
				return i;
		}
	}
	else {

	    for(i = E_Queue[E_FRONT]; i< E_Queue[E_SIZE]; i++) {

			if(Queue[i] == value)
			    return i;
		}
		for(i = 0; i <= E_Queue[E_REAR]; i++) {

		    if(Queue[i] == value)
		        return i;
		}
	}
	return -1;
}

stock internal_queue_replace({_STRONG_TAGS_}:Queue[], {_STRONG_TAGS_}:value1,
							{_STRONG_TAGS_}:value2,
							{_, QueueC}:E_Queue[E_QUEUE_DATA],
							tagid = tagof(E_Queue)) {

	new idx;
	if(tagid == tagof(QueueC:))
	    idx = internal_queue_search(Queue, value1, QueueC:E_Queue);
	else
	    idx = internal_queue_search(Queue, value1, E_Queue);
	if(idx != -1)
	    Queue[idx] = value2;
	return idx;
}

stock internal_queue_to_arr({_STRONG_TAGS_}:Queue[],
							{_, QueueC}:E_Queue[E_QUEUE_DATA],
							{_STRONG_TAGS_}:arr[], tagid = tagof(E_Queue)) {

	new i, j = 0;
	if(tagid != tagof(QueueC:) || E_Queue[E_REAR] >= E_Queue[E_FRONT]) {

	    for(i = E_Queue[E_FRONT]; i <= E_Queue[E_REAR]; i++)
	        arr[j++] = Queue[i];
	 } else {

	    for(i = E_Queue[E_FRONT]; i < E_Queue[E_SIZE]; i++)
	        arr[j++] = Queue[i];

		for(i = 0; i <= E_Queue[E_REAR]; i++)
		    arr[j++] = Queue[i];
	}
	return 1;
}

stock internal_arr_to_queue({_STRONG_TAGS_}:Queue[],
							{_, QueueC}:E_Queue[E_QUEUE_DATA],
							{_STRONG_TAGS_}:arr[],
							size = sizeof(arr)) {
	new i;
	for(i = 0, E_Queue[E_FRONT] = 0, E_Queue[E_REAR] = size - 1,
		E_Queue[E_SIZE] = size; i< size; i++)
		Queue[i] = arr[i];
	return 1;
}

stock internal_queue_del_element({_STRONG_TAGS_}:Queue[], {_STRONG_TAGS_}:value,
							{_, QueueC}:E_Queue[E_QUEUE_DATA],
							tagid = tagof(E_Queue)) {

	new idx, i;
	if(tagid == tagof(QueueC:)) {

	    idx = internal_queue_search(Queue, value, QueueC:E_Queue);
	    if(idx != -1) {

			if(E_Queue[E_REAR] >= E_Queue[E_FRONT]) {

			    for(i = idx + 1; i <= E_Queue[E_REAR]; i++)
			        Queue[i-1] = Queue[i];
			}
			else {

				for(i = idx + 1; i < E_Queue[E_SIZE]; i++)
				    Queue[i-1] = Queue[i];

				Queue[i-1] = Queue[0];
				for(i = 1; i <= E_Queue[E_REAR]; i++)
				    Queue[i-1] = Queue[i];
			}
			E_Queue[E_REAR]--;
		}
	} else {

	    idx = internal_queue_search(Queue, value, E_Queue);
	    if(idx != -1) {

	        for(i = idx + 1; i <= E_Queue[E_REAR]; i++)
	            Queue[i - 1] = Queue[i];
			E_Queue[E_REAR]--;
		}
	}
	return idx;
}

//For debugging purposes.
stock internal_print_queue({_STRONG_TAGS_}:Queue[],
							{_, QueueC}:E_Queue[E_QUEUE_DATA],
							tagid = tagof(E_Queue),
							qtagid = tagof(Queue)) {

	new i, sformatSpecifier[3];
	if(qtagid == tagof(Float:))
	    sformatSpecifier = "%f";
	else
	    sformatSpecifier = "%d";
	if(tagid != tagof(QueueC:) || E_Queue[E_REAR] >= E_Queue[E_FRONT]) {

	    for(i = E_Queue[E_FRONT]; i <= E_Queue[E_REAR]; i++)
	        printf(sformatSpecifier, Queue[i]);
	} else {

	    for(i = E_Queue[E_FRONT]; i < E_Queue[E_SIZE]; i++)
	        printf(sformatSpecifier, Queue[i]);

		for(i = 0; i <= E_Queue[E_REAR]; i++)
		    printf(sformatSpecifier, Queue[i]);
	}
	return 1;
}

