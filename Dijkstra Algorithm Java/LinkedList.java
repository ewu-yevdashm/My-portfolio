//MARK YEVDASH

/*
 * Linked List Class: this list is used for packages.
 */
//-----------------------------------------------------------------------------------------------------------------------
public class LinkedList {
	/*
	 * Node class within the linked list class as standard for linked lists
	 */
	public class Node
	{	
		public int weight;
		public int dest;
		public Node next;
			
		public Node(int weight, int dest, Node next)
		{
			this.weight = weight;
			this.dest = dest;
			this.next = next;
		}
		public Node(int weight, int dest) {
			
			this.weight = weight;
			this.dest = dest;
		}
		
		public int getWeight(){
			return this.weight;
		}
		public int getDest(){
			return this.dest;
		}

	}	
	private Node head;
	private int size;
	public int distanceTo;
	public int vertex;
	public LinkedList prev = null;
	
	//-----------------------------------------------------------------------------------------------------------------------
	public LinkedList(int vertex, LinkedList prev)
	{
		this.head = null;
		this.size = 0;
		this.distanceTo = 1000000000;
		this.vertex = vertex;
		this.prev = prev;
	}
	
	//-----------------------------------------------------------------------------------------------------------------------
	/*
	 * Add method: adding new items to the list.
	 */
	public void add(int weight, int dest)
	{
		Node newNode = new Node(weight, dest);
		if(size == 0) //is empty
		{
			head = newNode;
		}
		else
		{
			Node curr = head;
			while(curr.next != null)
			{
				curr = curr.next;
			}
		curr.next = newNode;
		}
	size++;	
	}
		
	//-----------------------------------------------------------------------------------------------------------------------
		/*
		 * checking if the list is empty
		 */
	
	public boolean isEmpty()
	{
		return head == null;
	}
	
	
	//for checking reasons.
	public void printWeights(){
		
			
			Node curr = head;
			
			while (curr.next != null){
				
				System.out.println(curr.getWeight());
				curr = curr.next;
			}
			
			System.out.println(curr.getWeight());
	}

	public int[] getConnections() {
		
		Node curr = head;
		int i = 0;
		int [] connections = new int[this.size];
		
		while (curr != null){
			
			connections[i] = curr.getDest();
			i ++;
			curr = curr.next;
		}
		
		return connections;
	}
	
	public int[] getWeights() {
		
		Node curr = head;
		int i = 0;
		int [] weights = new int[this.size];
		
		while (curr != null){
			
			weights[i] = curr.getWeight();
			i ++;
			curr = curr.next;
		}
		
		return weights;
	}

}
