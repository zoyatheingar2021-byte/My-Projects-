package chp8;
import java.util.*;
public class eg1 {

	public static void main(String[] args) {
		Staff st=new Staff(); //creating object
		st.setName();
		st.setAge();
		System.out.println("Name:"+st.getName());
		System.out.println("Age: "+st.getAge());
		
		Staff st1=new Staff();
		st1.setName();
		st1.setAge();
		System.out.println("Name:"+st1.getName());
		System.out.println("Age: "+st1.getAge());
	}
	

}
class Staff
{
	private String name;
	private int age;
	private Scanner scan=new Scanner (System.in);
	public void setName()
	{
		System.out.println("Enter Staff Name: ");
		name=scan.nextLine();
	}
	public void setAge()
	{
		System.out.println("Enter Staff Age: ");
		age=scan.nextInt();
	}
	public String getName()
	{
		return name;
	}
	public int getAge()
	{
		return age;
	}
		
}

