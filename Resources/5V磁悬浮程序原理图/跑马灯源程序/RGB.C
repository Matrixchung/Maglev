#include<reg52.h>
#include<intrins.h>  
		   void RGBA(unsigned int ta); 
		  void RGBB(	unsigned int tb);
		  void RGBC (unsigned int tc);	  
		   void rgbd(unsigned int td);	  
		   void RGBE(unsigned int te);	 
		   void rgbf(unsigned int tf);	
		   void rgbg(unsigned int tg);	 
			void rgbh(unsigned int th);						   
			void rgbl(unsigned int tl);	 
	#define uchar unsigned char
	 sfr PLC=0X97;
	 sbit Send_Dat=P3^3;
	 unsigned char RGB_BUF[24];
		 void delay();
			 uchar  n;
			uchar r;
			  unsigned char *p;
			   void Delay500us()	;
	  
		 uchar a []={ 0xff,0x00,0x00} ;	 
		 uchar b []={ 0x00,0xff,0x00} ;	 
		 uchar c []={ 0x00,0x00,0xff} ;	
		 uchar d []={ 0xff,0xff,0x00} ;	 
		 uchar e []={ 0xff,0x00,0xff} ;	 
		 uchar f []={ 0x00,0xff,0xff} ;	 
		 uchar g []={ 0xff,0xff,0xff} ;	 
		 uchar h []={ 0x00,0x00,0x00} ;
										
			 uchar shuzud [8] [3]={  
	  { 0xff,0x00,0x00},	  
		{ 0x00,0xff,0x00} ,	   
		{ 0x00,0x00,0xff} ,	   
		{ 0xff,0xff,0x00} ,	   
		{ 0xff,0x00,0xff} ,	  
		{ 0x00,0xff,0xff} ,	   
		{ 0xff,0xff,0xff} ,	   
		{ 0x00,0x00,0x00} ,
		                        };

		   void Delay1us()	;
		   sfr P3M1 = 0xb1;
       sfr P3M0 = 0xb2;
		 
struct shuzu 
		   {
			  uchar a [3];
			  uchar b[3] ;			
	     };	
			 
			struct	shuzu;
void delays(uchar t);

		void Send_A_bit(uchar v)
{   
       if (v==1)
       {   
              Send_Dat=1;
             	Delay1us();	
              Send_Dat=0;                    
       }
       else
       {
              Send_Dat=1;            
              Send_Dat=0;           
			    Delay1us();	      
       }                
}


		 void Send_192bits( uchar *p1,uchar z)                      
{
       unsigned int i=0;	uchar s; uchar x; unsigned char k; 	    	  
					 s=8;
      		   x=0;		
					 						   		 
	   for   (i=x;i<s;i++)
       {
              RGB_BUF[i]=p1[0]>>(i-x)&0x01;       
       }
					   s=s+8;
					   x=x+8;
			 
              for   (i=x;i<s;i++)
       {
              RGB_BUF[i]=p1[1]>>(i-x)&0x01;      
       }				   
	   		     s=s+8;
					   x=x+8;
			 
					   for   (i=x;i<s;i++)
       {
              RGB_BUF[i]=p1[2]>>(i-x)&0x01;			 
       }						
						 s=s+8;
					   x=x+8; 
					   		 
					for (k=0;k<z;k++)	  
				  {																			     	    				
			 for (i=0;i<24;i++)
      {
              Send_A_bit(RGB_BUF[i]);
       }			 			 			 			 			 
			 	 }			 			 			 			 			 	
}


		void main()
		{
				 P3M0 = 0x00;
         P3M1 = 0x00;
			while(1)
			   {

		  		RGBC(10); 
				  delays(10 );
					 
					RGBA();
				  delays(10 );
					 
			  	RGBB(); 
          delays(10 );					
					 
	

		  		RGBE();	 
				    delays(10 );
				   	

					 
		      rgbg(5);		
			      delays(10 );

			      rgbh(10);
            delays(10 );
					
						
						
						
				      rgbl(10);
            delays(100 );	 

			   
	 }
}
		

/////////////////////////////////////////////////////////////////////////////////////3
      void RGBC(unsigned int tc)	   
	  {

				   Send_192bits(shuzud[0],18);
			   	 delays(tc);
				   Send_192bits(shuzud[1],18);
			   	 delays(tc);
				   Send_192bits(shuzud[2],18);
			   	 delays(tc);
				   Send_192bits(shuzud[3],18);
			   	 delays(tc);
				   Send_192bits(shuzud[4],18);
			   	 delays(tc);
				   Send_192bits(shuzud[5],18);
			   	 delays(tc);
					 Send_192bits(shuzud[7],18);
			   	 delays(tc);
		}



/////////////////////////////////////////////////////////////////////////////////////1
	  void RGBA(unsigned int ta)			 
	  {				   
					 Send_192bits(shuzud[7],19);
			   	 delays(10);	
			
						for (n=0;n<19;n++)
				   	  {		  	 
			    		 Send_192bits(shuzud[0],n);
			   	 delays(2);				   
              }
			   	 delays(10);	
							
						for (n=0;n<19;n++) 
				   	  {		  	 
			    		 Send_192bits(shuzud[1],n);
			   	 delays(2);       
				 	 }
			   	 delays(10);	
					 
						for (n=0;n<19;n++) 
				   	  {		  	 
			    		 Send_192bits(shuzud[2],n);
			   	 delays(2);       
				 	 }
					 delays(10);	
					 
						for (n=0;n<19;n++) 
				   	  {		  	 
			    		 Send_192bits(shuzud[3],n);
			   	 delays(2);       
				 	 }
					 delays(10);	
					 
						for (n=0;n<19;n++) 
				   	  {		  	 
			    		 Send_192bits(shuzud[4],n);
			   	 delays(2);       
				 	 }
					 delays(10);	
					 
						for (n=0;n<19;n++) 
				   	  {		  	 
			    		 Send_192bits(shuzud[5],n);
			   	 delays(2);       
				 	 }
					 delays(10);	
				 
						for (n=0;n<19;n++) 
				   	  {		  	 
			    		 Send_192bits(shuzud[7],n);
			   	 delays(2);       
				 	 }
					 delays(10);	
	
					 
		}

/////////////////////////////////////////////////////////////////////////////////////2		
			void	  RGBB( unsigned int tb		 ) 	 
	  {				
	  							 unsigned int t;
								  unsigned int w=65000;

          			      for (t=0;t<	w;t++);	  
	  					        Send_192bits(shuzud[2],9);
			   	 				    for (t=0;t<	w;w++);				   					 
						 		      Send_192bits(shuzud[1],8);
			 								for (t=0;t<	w;t++);
						   		    Send_192bits(shuzud[0],7);			   	
										 	for (t=0;t<	w;t++);
								      Send_192bits(shuzud[5],6);			   	
										 	for (t=0;t<	w;t++);
	  							    Send_192bits(shuzud[4],5);		   	
										 	for (t=0;t<	w;t++);	  
	  							    Send_192bits(shuzud[3],4);			   	
										 	for (t=0;t<	w;t++);
	  					 			  Send_192bits(shuzud[2],3);			   	
										 	for (t=0;t<	w;t++);
										  Send_192bits(shuzud[1],2);			   	
										 	for (t=0;t<	w;t++);										   
										  Send_192bits(shuzud[0],1);			   	
										 	for (t=0;t<	w;t++);
	 						     
			 }
		


/////////////////////////////////////////////////////////////////////////////////////5
	   	  void RGBE(unsigned int te)
   {		 		
				  unsigned int t;
				    unsigned int w =65000;									
	  								   Delay500us();
						 		      Send_192bits(shuzud[5],18);			 								
	  								   Delay500us();										  
						   		    Send_192bits(shuzud[4],17);			   											 	 
	  								  Delay500us();										    
								      Send_192bits(shuzud[3],16);			   											 	
	  								   Delay500us();										   
	  							    Send_192bits(shuzud[2],15);	   										 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[1],14);			   											 
	  								   Delay500us();																	   
	  							    Send_192bits(shuzud[0],13);
	  								   Delay500us();
						 		      Send_192bits(shuzud[5],12);			 								
	  								   Delay500us();										  
						   		    Send_192bits(shuzud[4],11);			   											 	 
	  								  Delay500us();										    
								      Send_192bits(shuzud[3],10);			   											 	
	  								   Delay500us();										   
	  							    Send_192bits(shuzud[2],9);	   										 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[1],8);			   											 
	  								   Delay500us();																	   
	  							    Send_192bits(shuzud[0],7);
	  								   Delay500us();
						 		      Send_192bits(shuzud[5],6);			 								
	  								   Delay500us();										  
						   		    Send_192bits(shuzud[4],5);			   											 	 
	  								  Delay500us();										    
								      Send_192bits(shuzud[3],4);			   											 	
	  								   Delay500us();										   
	  							    Send_192bits(shuzud[2],3);	   										 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[1],2);			   											 
	  								   Delay500us();																	   
	  							    Send_192bits(shuzud[0],1);
		 		 
		 
	  								   delays(100);

		}


/////////////////////////////////////////////////////////////////////////////////////6



/////////////////////////////////////////////////////////////////////////////////////7
			  void rgbg(unsigned int tg)
	 {
					  unsigned int t;
				    unsigned int w =65000;								  								 	
	  				   Delay500us();					
						 		 Send_192bits(shuzud[0],18);
										 delays(tg);
		 
											   Delay500us();										  
						   		 Send_192bits(shuzud[0],18);
			 							  Delay500us();										  
						   		 Send_192bits(shuzud[0],16);	
	  								   Delay500us();										  			  
						   		 Send_192bits(shuzud[0],14);		   										 	 
	  								  Delay500us();	
						   		 Send_192bits(shuzud[0],12);			   										 	 
	  								  Delay500us();										    
								   Send_192bits(shuzud[1],10);			   											 	
	  								   Delay500us();	
	  							    Send_192bits(shuzud[1],8);		   										 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[0],6);			   											 
	  								   Delay500us();										 								   
	  							    Send_192bits(shuzud[0],4);			   	
										  Delay500us(); 	  								  								  
	  							    Send_192bits(shuzud[0],2);			   											 									 
	  								   delays(tg);
											 
											 Delay500us();										  
						   		 Send_192bits(shuzud[0],318);
			 							  Delay500us();										  
						   		 Send_192bits(shuzud[0],16);	
	  								   Delay500us();										  			  
						   		 Send_192bits(shuzud[0],14);			   											 	 
	  								  Delay500us();	
						   		 Send_192bits(shuzud[1],12);										 	 
	  								  Delay500us();										    
								   Send_192bits(shuzud[1],10);			   											 	
    								   Delay500us();										   
	  							    Send_192bits(shuzud[1],8);										 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[1],6);			   											 
	  								   Delay500us();										 								   
	  							    Send_192bits(shuzud[0],4);			   	
										  Delay500us(); 	  								  									  
	  							    Send_192bits(shuzud[0],2);			   											 										 
	  								   delays(tg);
											 
								   Delay500us();										  
						   		 Send_192bits(shuzud[0],18);
			 							  Delay500us();									  
						   		 Send_192bits(shuzud[0],16);	
	  								   Delay500us();										  			  
						   		 Send_192bits(shuzud[1],14);			   											 	 
	  								  Delay500us();	
						   		 Send_192bits(shuzud[1],12);										 	 
	  								  Delay500us();										    
								   Send_192bits(shuzud[1],10);			   											 	
	  								   Delay500us();										   
	  							    Send_192bits(shuzud[1],8);			   											 	 
	  								   Delay500us();		  								    
											 Send_192bits(shuzud[1],6);			   										 
	  								   Delay500us();									
	  							    Send_192bits(shuzud[1],4);			   	
										  Delay500us(); 	  								  
	  							    Send_192bits(shuzud[0],2);			   											 			   																  										 
	  								   delays(tg);
											 
	                    Delay500us();										  
						   		 Send_192bits(shuzud[0],18);
			 							  Delay500us();										  
						   		 Send_192bits(shuzud[1],16);	
	  								   Delay500us();										  			  
						   		 Send_192bits(shuzud[1],14);										 	 
	  								  Delay500us();	
						   		 Send_192bits(shuzud[1],12);			   											 	 
	  								  Delay500us();										    
								   Send_192bits(shuzud[1],10);										 	
	  								   Delay500us();										   
	  							    Send_192bits(shuzud[1],8);			   										 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[1],6);			   											 
	  								   Delay500us();									
	  							    Send_192bits(shuzud[1],4);			   	
										  Delay500us(); 	  								  								  
	  							    Send_192bits(shuzud[1],2);			   										 								  
	  								   delays(tg);
											 
				 				   Delay500us();										  
						   		 Send_192bits(shuzud[1],18);
			 							  Delay500us();										  
						   		 Send_192bits(shuzud[1],16);	
	  								   Delay500us();										  			  
						   		 Send_192bits(shuzud[1],14);			   											 	 
	  								  Delay500us();	
						   		 Send_192bits(shuzud[1],12);			   											 	 
									 Delay500us();										    
								   Send_192bits(shuzud[1],10);			   											 	
	  								   Delay500us();										   
	  							    Send_192bits(shuzud[1],8);			   											 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[1],6);			   											 
	  								   Delay500us();																	   
	  							    Send_192bits(shuzud[1],4);			   	
										  Delay500us();
	  							    Send_192bits(shuzud[1],2);										 		   																  										 
	  								   delays(tg);	
											 	  								   delays(100);
										
				 				   Delay500us();										  
						   		 Send_192bits(shuzud[2],18);
			 							  Delay500us();										  
						   		 Send_192bits(shuzud[1],16);	
	  								   Delay500us();									  			  
						   		 Send_192bits(shuzud[1],14);			   											 	 
	  								  Delay500us();	
						   		 Send_192bits(shuzud[1],12);										 	 
	  								  Delay500us();										    
								   Send_192bits(shuzud[1],10);			   											 	
	  								   Delay500us();										   
	  							    Send_192bits(shuzud[1],8);			   											 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[1],6);			   											 
	  								   Delay500us();								   
	  							    Send_192bits(shuzud[1],4);			   	
										  Delay500us(); 	  								  
	  							    Send_192bits(shuzud[1],2);										 	   																	 
	  								   delays(tg);

				 				   Delay500us();									  
						   		 Send_192bits(shuzud[2],18);
			 							  Delay500us();									  
						   		 Send_192bits(shuzud[2],16);	
	  								   Delay500us();								  			  
						   		 Send_192bits(shuzud[1],14);			   										 	 
	  								  Delay500us();	
						   		 Send_192bits(shuzud[1],12);			   											 	 
	  								  Delay500us();										    
								   Send_192bits(shuzud[1],10);			   											 	
	  								   Delay500us();										   
	  							    Send_192bits(shuzud[1],8);			  									 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[1],6);			   											 
	  								   Delay500us();											   										 																	   
	  							    Send_192bits(shuzud[1],4);			   	
										  Delay500us();
	  							    Send_192bits(shuzud[2],2);			   											 								  
	  								   delays(tg);
											 
									 	   Delay500us();									  
						   		 Send_192bits(shuzud[2],18);
			 							  Delay500us();										  
						   		 Send_192bits(shuzud[2],16);	
	  								   Delay500us();										  			  
						   		 Send_192bits(shuzud[2],14);			   											 	 
	  								  Delay500us();	
						   		 Send_192bits(shuzud[1],12);			   											 	 
	  								  Delay500us();									    
								   Send_192bits(shuzud[1],10);			   											 	
	  								   Delay500us();										   
	  							    Send_192bits(shuzud[1],8);			   											 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[1],6);			   											 
	  								   Delay500us();										   																   
	  							    Send_192bits(shuzud[2],4);			   	
										  Delay500us(); 	  								  
	  							    Send_192bits(shuzud[2],2);			   											 	   																  										 
	  								   delays(tg);
											 
									  	 Delay500us();	
								   		 Send_192bits(shuzud[2],18);
			 							  Delay500us();										  
						   		 Send_192bits(shuzud[2],16);	
	  								   Delay500us();										  			  
						   		 Send_192bits(shuzud[2],24);										 	 
	  								  Delay500us();	
						   		 Send_192bits(shuzud[2],12);			   											 	 
	  								  Delay500us();										    
								   Send_192bits(shuzud[1],10);			   											 	
	  								   Delay500us();									   
	  							    Send_192bits(shuzud[1],8);		   										 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[2],6);			   											 
	  								   Delay500us();										   									 																   
	  							    Send_192bits(shuzud[2],4);			   	
										  Delay500us(); 	  								  
	  							    Send_192bits(shuzud[2],2);			   											 										 
	  								   delays(tg);
											 
									  	 Delay500us();										  
						   		 Send_192bits(shuzud[2],18);
			 							  Delay500us();										  
						   		 Send_192bits(shuzud[2],16);	
	  								   Delay500us();										  			  
						   		 Send_192bits(shuzud[2],14);										 	 
	  								  Delay500us();	
						   		 Send_192bits(shuzud[2],12);		   											 	 
	  								  Delay500us();									    
								   Send_192bits(shuzud[2],10);			   											 	
	  								   Delay500us();										   
	  							    Send_192bits(shuzud[2],8);			   											 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[2],6);			   											 
	  								   Delay500us();											   										 																	   
	  							    Send_192bits(shuzud[2],4);			   	
										  Delay500us(); 	  								  
	  							    Send_192bits(shuzud[2],2);			   											 		   																  										 
	  								   delays(tg);

	}


/////////////////////////////////////////////////////////////////////////////////////8
		    void rgbh(unsigned int th)
   {		 		
				  unsigned int t;
				    unsigned int w =65000;								
	  								   Delay500us();	  							   					 
						 		 Send_192bits(shuzud[0],18);			 								
	  								   Delay500us();										  
						   		 Send_192bits(shuzud[1],15);			   											 	 
	  								  Delay500us();										    
								   Send_192bits(shuzud[2],12);			   											 	
	  								   Delay500us();										   
	  							    Send_192bits(shuzud[3],9);			   											 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[4],6);			   											 
	  								   Delay500us();																	   
	  							    Send_192bits(shuzud[5],3);	   										 
	  								   delays(th);
	  								   Delay500us();		
						 		 Send_192bits(shuzud[1],18);			 								
	  								   Delay500us();										  
						   		 Send_192bits(shuzud[2],15);			   										 	 
	  								  Delay500us();									    
								   Send_192bits(shuzud[3],12);		   									 	
	  								   Delay500us();									   
	  							    Send_192bits(shuzud[4],9);			   											 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[5],6);			   											 
	  								   Delay500us();											   										 																	   
	  							    Send_192bits(shuzud[0],3);			   											 
	  								   delays(th);						  
	  								   Delay500us();	
						 		 Send_192bits(shuzud[2],18);			 								
	  								   Delay500us();									  
						   		 Send_192bits(shuzud[3],15);			   										 	 
	  								  Delay500us();										    
								   Send_192bits(shuzud[4],12);			   											 	
	  								   Delay500us();										   
	  							    Send_192bits(shuzud[5],9);			   											 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[0],6);			   											 
	  								   Delay500us();																	   
	  							    Send_192bits(shuzud[1],3);										 
	  								   delays(th);
	  								   Delay500us();
						 		 Send_192bits(shuzud[3],18);			 								
	  								   Delay500us();										  
						   		 Send_192bits(shuzud[4],15);			   											 	 
	  								  Delay500us();										    
								   Send_192bits(shuzud[5],12);										 	
	  								   Delay500us();										   
	  							    Send_192bits(shuzud[0],9);										 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[1],6);										 
	  								   Delay500us();								   
	  							    Send_192bits(shuzud[2],3);			   											 
	  								   delays(th);
							  	   Delay500us();		  								   					 
						 		 Send_192bits(shuzud[4],18);			 								
	  								   Delay500us();										  
						   		 Send_192bits(shuzud[5],15);									 	 
	  								  Delay500us();										    
								   Send_192bits(shuzud[0],12);			   											 	
	  								   Delay500us();										   
	  							    Send_192bits(shuzud[1],9);	   										 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[2],6);			   											 
	  								   Delay500us();																	   
	  							    Send_192bits(shuzud[3],3);			   											 
	  								   delays(th);
									   Delay500us();		  							   					 
						 		 Send_192bits(shuzud[5],18);		 								
	  								   Delay500us();										  
						   		 Send_192bits(shuzud[0],15);			   											 	 
	  								  Delay500us();									    
								   Send_192bits(shuzud[1],12);			   											 	
	  								   Delay500us();									   
	  							    Send_192bits(shuzud[2],9);									 	 
	  								   Delay500us();	  								   
	  							    Send_192bits(shuzud[3],6);		   										 
	  								   Delay500us();										   							   
	  							    Send_192bits(shuzud[4],3);		   										 
	  								   //delays(th);

   }
/////////////////////////////////////////////////////////////////////////////////////8
		    void rgbl(unsigned int tl)
   {		 		
				  unsigned int t;
				    unsigned int w =65000;								
	  								   Delay500us();	  							   					 
						 		 Send_192bits(shuzud[7],18);			 								
	  								   Delay500us();										  
						   		 Send_192bits(shuzud[7],15);			   											 	 
	  								  Delay500us();										    
								   Send_192bits(shuzud[7],12);			   											 	
	  								   Delay500us();										   
	  							    Send_192bits(shuzud[7],9);			   											 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[7],6);			   											 
	  								   Delay500us();																	   
	  							    Send_192bits(shuzud[0],3);	   										 
	  								   delays(tl);
	  								   Delay500us();		
						 		 Send_192bits(shuzud[7],18);			 								
	  								   Delay500us();										  
						   		 Send_192bits(shuzud[7],15);			   										 	 
	  								  Delay500us();									    
								   Send_192bits(shuzud[7],12);		   									 	
	  								   Delay500us();									   
	  							    Send_192bits(shuzud[7],9);			   											 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[0],6);			   											 
	  								   Delay500us();											   										 																	   
	  							    Send_192bits(shuzud[1],3);			   											 
	  								   delays(tl);						  
	  								   Delay500us();	
						 		 Send_192bits(shuzud[7],18);			 								
	  								   Delay500us();									  
						   		 Send_192bits(shuzud[7],15);			   										 	 
	  								  Delay500us();										    
								   Send_192bits(shuzud[7],12);			   											 	
	  								   Delay500us();										   
	  							    Send_192bits(shuzud[0],9);			   											 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[1],6);			   											 
	  								   Delay500us();																	   
	  							    Send_192bits(shuzud[2],3);										 
	  								   delays(tl);
	  								   Delay500us();
						 		 Send_192bits(shuzud[7],18);			 								
	  								   Delay500us();										  
						   		 Send_192bits(shuzud[7],15);			   											 	 
	  								  Delay500us();										    
								   Send_192bits(shuzud[0],12);										 	
	  								   Delay500us();										   
	  							    Send_192bits(shuzud[1],9);										 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[2],6);										 
	  								   Delay500us();								   
	  							    Send_192bits(shuzud[3],3);			   											 
	  								   delays(tl);
							  	   Delay500us();		  								   					 
						 		 Send_192bits(shuzud[7],18);			 								
	  								   Delay500us();										  
						   		 Send_192bits(shuzud[0],15);									 	 
	  								  Delay500us();										    
								   Send_192bits(shuzud[1],12);			   											 	
	  								   Delay500us();										   
	  							    Send_192bits(shuzud[2],9);	   										 	 
	  								   Delay500us();		  								   
	  							    Send_192bits(shuzud[3],6);			   											 
	  								   Delay500us();																	   
	  							    Send_192bits(shuzud[4],3);			   											 
	  								   delays(tl);
									   Delay500us();		  							   					 
						 		 Send_192bits(shuzud[0],18);		 								
	  								   Delay500us();										  
						   		 Send_192bits(shuzud[1],15);			   											 	 
	  								  Delay500us();									    
								   Send_192bits(shuzud[2],12);			   											 	
	  								   Delay500us();									   
	  							    Send_192bits(shuzud[3],9);									 	 
	  								   Delay500us();	  								   
	  							    Send_192bits(shuzud[4],6);		   										 
	  								   Delay500us();										   							   
	  							    Send_192bits(shuzud[5],3);

	  								   //delays(tl);

   }












		
	void delay()
{
unsigned int i,j;
       for (i=0;i<250;i++)
       {
              for (j=0;j<200;j++);
       }
}


//大延时函数十几秒
void delays(uchar t)
{
		   uchar p;


	 for (p=0; p<t;p++)
	   {
       delay();
      	
      				  }
       
}



	   void Delay1us()		//@27.000MHz
{
	unsigned char i;

	_nop_();
	i = 4;
	while (--i);
}


   void Delay500us()		//@27.000MHz
{ 
unsigned char i, j;

	i = 14;
	j = 30;
	do
	{
		while (--j);
	} while (--i);
}



