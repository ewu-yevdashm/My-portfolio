//MARK YEVDASH



public class MinHeap {

	static LinkedList [] heap;
	static int size = 0;
	static int capasity;
	int vertex;
	int used;

	public MinHeap(int capasity){
		
		this.size = 0;
		this.capasity = capasity;
		heap =  new LinkedList[capasity + 1]; //CHECK THAT + 1 IS RIGHT
	}//end minHeap
    
	public void newElement(LinkedList list){
		
		if(this.size == 0){
			heap[1] = list;
			this.size ++;
		}//end if
		else{
			heap[size + 1] = list;
			min_heapify(1);
			this.size ++;
		}//end else
	}//end new Element
	public LinkedList extractMin(){
		
		if(size == 0){
			return null;
		}
		else{
			
			LinkedList min = heap[1];
			heap[1] = heap[size];
			this.size --;
			min_heapify(1);
	
			return min;				
		}//end else
	}//end extractMin

	private static void min_heapify(int parent)
	{
		int leftChild = 2 * parent;
		int rightChild = leftChild + 1;
		int smallest = 0;        
		
		if (rightChild <= size && heap[rightChild].distanceTo < heap[parent].distanceTo)
			smallest = rightChild;
		else
			smallest = parent;
		
		if (leftChild <= size && heap[leftChild].distanceTo < heap[smallest].distanceTo)
			smallest = leftChild;
		
		if (smallest != parent)
		{
			LinkedList temp = heap[parent];
			heap[parent] = heap[smallest];
			heap[smallest] = temp;
            
			//Recursive call to continue to bottom of the tree.
			min_heapify(smallest);
		}//end if
	}//end min heapify	

	public void print() {
	
		for(int i = 1; i < this.size+1; i ++){
			
			System.out.println(heap[i].distanceTo);
		}	
	}//end print
	public boolean isEmpty() {
		
		return size == 0;
	}//end isEmpty

	void shiftUp() {
       
		for(int i = size/2; i >= 1; i--){
			
			min_heapify(i);
		}//end for
    }//end shift up	
}
