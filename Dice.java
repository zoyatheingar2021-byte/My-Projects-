package chp7;
import java.util.*;
public class eg1 {

	public static void main(String[] args) {
		Random diceValues=new Random();
		System.out.println("You have six throw of a pair of dices.\nYour objective is to get a double six.Here goes.....\n");
		String[]goes= {"First","Second","Third","Fourth","Fifth","Sixth"};
		int die1=0;
		int die2=0;
		for (int i=0; i<goes.length; i++)
		{
			die1=diceValues.nextInt(6)+1;
			die2=diceValues.nextInt(6)+1;
			System.out.println(goes[i]+" throw: "+die1+","+die2);
			if (die1+die2==12)
			{
				System.out.println("\tYou Win!!");
				return;
			}
		}
		System.out.println("\tSorry, You Lost!");
		return;
		
	}

}
