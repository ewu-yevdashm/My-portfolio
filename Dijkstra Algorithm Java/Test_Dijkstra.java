//MARK YEVDASH
//00637499
//PROG 2


import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class Test_Dijkstra {

	static int startingPoint;
	
	
	public static void main(String [] args){
		
		String filename = args[0]; 
					
		String split[] = null;
		
		try {
			split = readFile(filename);
		} catch (IOException e) {
			System.out.println("The File was NOT found.");
		}
		
//----------------------------------------------------------------STRING PARSING--------------------------------------------
		LinkedList [] array = new LinkedList[split.length];
		
		
		for(int i = 0; i < split.length; i ++){
			
			StringBuilder sb = new StringBuilder();
			
			int vertex = 0;
			sb.append(split[i]);
			
			//get colon index
			int indexColon = split[i].indexOf(':');
			String vertexString = sb.substring(0, indexColon);
			
			//main vertex
			vertex = Integer.parseInt(vertexString);
			
		
			LinkedList list = new LinkedList(vertex, null);
			
			//delete the vertex number and the colon
			sb.delete(0, indexColon + 1);
	
			
			while(sb.length() != 0 && sb.length() != 1)
			{
				int dest = 0;
				int weight = 0;
				
				int comaIndex = sb.indexOf(",", 0);
				
				if(comaIndex > -1)
				{
					String destString = sb.substring(0, comaIndex);
					dest = Integer.parseInt(destString);
					sb.delete(0, comaIndex + 1);
	
					int semiColonIndex = sb.indexOf(";", 0);
					
					if(semiColonIndex > -1)
					{	
						String weightString = sb.substring(0, semiColonIndex);
						weight = Integer.parseInt(weightString);
						sb.delete(0, semiColonIndex + 1);
					}
					else
					{
						String weightString = sb.substring(0);
						weight = Integer.parseInt(weightString);
						sb.deleteCharAt(0);
					}					
				}
				list.add(weight, dest);
			}
			array[vertex] = list;
				
			sb = null;			
		}
			
//----------------------------------------------------------------STRING PARSING--------------------------------------------\\
		
		
		int source = Integer.parseInt(args[1]); 
		
		dijkstra(split.length, array, source);
		
		outputResults(array, source);
	}

	private static void outputResults(LinkedList[] array, int source) {
		
		for(int i = 0; i < array.length; i ++){
		
			if(array[i].vertex != source){
				
				System.out.print("[" + i + "]");			
				StringBuilder sb = new StringBuilder();
				

				if(array[i].distanceTo != 1000000000)
				{				
					System.out.print("shortest path:");	
					sb.append(')');
					sb.insert(0, array[i].vertex);
					LinkedList previous = array[i].prev;
			
					boolean flag = false;		
					while(array[i].prev != null && previous != null && flag == false){
		
						if(previous.vertex == array[source].vertex)
							flag = true;

						sb.insert(0, ',');
						sb.insert(0, previous.vertex);
						previous = previous.prev;
												
					}					
					sb.insert(0, '(');
					sb.append(" shortest distance:");
					sb.append(array[i].distanceTo);
			
					System.out.println(sb);
				}
				else
					System.out.println("unreachable");	
			}
		}//end if	
	}

	
	private static void dijkstra(int numVerticies, LinkedList[] array, int source) {
			
		MinHeap mh = new MinHeap(numVerticies);
		
		//set the distance to the starting element 0
		array[source].distanceTo = 0;
		
		for(int i = 0; i < array.length; i ++){
			
			mh.newElement(array[i]);
			
		}
		
		int[] sourceConnections = array[source].getConnections();
		int[] sourceWeights = array[source].getWeights();
		
		for(int i = 0; i < sourceConnections.length; i ++){
			
			if(array[sourceConnections[i]].distanceTo > (sourceWeights[i] + array[source].distanceTo)){
				array[sourceConnections[i]].distanceTo = (sourceWeights[i] + array[source].distanceTo);
				
			}//end for sourceElement
		}
		
		LinkedList prev = array[source];
		while(!mh.isEmpty())
		{
			LinkedList min = mh.extractMin();
			min.prev = prev; // keep tracking of previous
			
			int[] connections = array[min.vertex].getConnections();
			int[] weights = array[min.vertex].getWeights();
			
			for(int i = 0; i < connections.length; i ++){
				
				if(array[connections[i]].distanceTo > (weights[i] + array[min.vertex].distanceTo)){
					
					array[connections[i]].distanceTo = (weights[i] + array[min.vertex].distanceTo);
					mh.shiftUp();
				
				}//end for sourceElement
			}
			
			prev = min;
		}
		
		//should take min off and now show the element in the array tha it belons too
	}

	private static String[] readFile(String fileName)throws IOException {
		
		//read in first line, level size
		BufferedReader br = new BufferedReader(new FileReader(fileName));
		StringBuilder sb = new StringBuilder();
		String line = br.readLine();
		while(line != null){
			
			sb.append(line);
			// sb.append(System.lineSeparator());
			sb.append(" ");
			line = br.readLine();
		}
		String everything = sb.toString();
		String split [] = everything.split(" ");
		br.close();
		return split;
		
	}//end readFile
		
}