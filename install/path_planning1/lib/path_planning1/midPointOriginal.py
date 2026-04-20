#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
import math
import numpy as np
from sdc_msgs.msg import Arr, Arrofarr, CoordinateList
from eufs_msgs.msg import ConeArrayWithCovariance
from nav_msgs.msg import Odometry
from tf_transformations import euler_from_quaternion

yaw = 0.0 
clus = True
cones=[]
left_cone_coordinates=[]
right_cone_coordinates=[]
min_distance=10000
min_left_coordinate=[-1000,-1000]
min_right_coordinate=[-1000,-1000]
temp=[]
car_coordinate=[0.0,0.0]

def distance(x,y):
    d=math.sqrt(((x[0]-y[0])**2)+((x[1]-y[1])**2))
    return d

class midPointOriginal(Node):
    def __init__(self):
        super().__init__('midPointOriginal')

        self.conePair_pub_ = self.create_publisher(Arrofarr, '/cone_pair', 1)
        self.odom_sub_ = self.create_subscription(Odometry, '/odom', self.odom_callback, 1)
        # self.cluster_sub_ = self.create_subscription(CoordinateList, '/Clusters', self.cluster_callback, 1)
        self.cluster_sub_ = self.create_subscription(ConeArrayWithCovariance, '/fusion/cones', self.cluster_callback, 1)
        self.get_logger().info("midPointOriginal node has been started.")

    def odom_callback(self, msg: Odometry):
        global yaw
        global car_coordinate

        car_coordinate[0] = msg.pose.pose.position.x
        car_coordinate[1] = msg.pose.pose.position.y
        quart = [msg.pose.pose.orientation.x, msg.pose.pose.orientation.y, msg.pose.pose.orientation.z, msg.pose.pose.orientation.w]
        (_, _, yaw) = euler_from_quaternion(quart)

    def cluster_callback(self, data: ConeArrayWithCovariance):
        global clus, car_coordinate, cones, min_distance, min_left_coordinate, min_right_coordinate, temp, left_cone_coordinates,right_cone_coordinates,yaw
        clus = False
        cones = []
        # for i in data.cone_coordinates:
        #     cones.append([i.x, i.y])
        for i in data.unknown_color_cones:
            cones.append([i.point.x, i.point.y])
        for i in data.blue_cones:
            cones.append([i.point.x, i.point.y])
        for i in data.yellow_cones:
            cones.append([i.point.x, i.point.y])
        


        #------------changing local cone coordinates to global-----------------------
        for i in range(len(cones)):
            temp_x = cones[i][0]
            temp_y = cones[i][1]
            cones[i][0] = temp_x*np.cos(yaw) - temp_y*np.sin(yaw)
            cones[i][1] = temp_x*np.sin(yaw) + temp_y*np.cos(yaw)
            cones[i][0]+= car_coordinate[0]
            cones[i][1]+= car_coordinate[1]
        
        if(len(left_cone_coordinates)==0 or len(right_cone_coordinates)==0):
            reference_x=car_coordinate[0]
            reference_y=car_coordinate[1]
        else:
            reference_x=(left_cone_coordinates[-1][0]+right_cone_coordinates[-1][0])/2
            reference_y=(left_cone_coordinates[-1][1]+right_cone_coordinates[-1][1])/2
        reference=[reference_x,reference_y]
        print("\n------------------------------------------------\n")
        print("reference=",reference)

        # print("before",cones)
        # cones_new=[]
        # flag=[1]*len(cones)
        # if(len(left_cone_coordinates)!=0 and len(right_cone_coordinates)!=0):
        # 	for i in range(0,len(cones)):
        # 		for j in range(0,len(left_cone_coordinates)-1):
        # 			p=[cones[i][0],cones[i][1]]
        # 			# print("distleft",left_cone_coordinates[j],p,distance(left_cone_coordinates[j],p))
        # 			# print("distright",right_cone_coordinates[j],p,distance(right_cone_coordinates[j],p))
        # 			if(distance(left_cone_coordinates[j],p)<1.5 or distance(right_cone_coordinates[j],p)<1.5 ):
        # 				# print("removed",p)
        # 				flag[i]=0
        # 				#cones_new.append(cones[i])

        # 	for i in range(0,len(cones)):
        # 		if(flag[i]==1):
        # 			cones_new.append(cones[i])
        # 	cones=cones_new
        print("cones: ", cones)
        myconecopy = cones;
        for i in range(0,len(myconecopy)):
            for j in range(i+1,len(myconecopy)):
                print("i=",cones[i]," j=",cones[j])
                for k in range(2):
                    print("i=",i, " j=", j, " k=", k)
                    if(k==1): 
                        temp = cones[i]
                        cones[i] = cones[j]
                        cones[j] = temp
                    z=True
                    vec1_i=1
                    vec1_j=1
                    vec2_i=1
                    vec2_j=1
                    angle=180
                    if(len(left_cone_coordinates)!=0):
                        if(distance(cones[i],left_cone_coordinates[-1])<1.4):
                            if(distance(cones[i], right_cone_coordinates[-1])<1.0):
                                continue
                            z=False
                            cones[i][0]=left_cone_coordinates[-1][0]
                            cones[i][1]=left_cone_coordinates[-1][1]
                            vec1_i=cones[j][0]-left_cone_coordinates[-1][0]
                            vec1_j=cones[j][1]-left_cone_coordinates[-1][1]
                            vec2_i=right_cone_coordinates[-1][0]-left_cone_coordinates[-1][0]
                            vec2_j=right_cone_coordinates[-1][1]-left_cone_coordinates[-1][1]
                        # elif(distance(cones[i],right_cone_coordinates[-1])<1.4):
                        #     z=False
                        #     cones[i][0]=right_cone_coordinates[-1][0]
                        #     cones[i][1]=right_cone_coordinates[-1][1]
                        #     vec1_i=cones[j][0]-right_cone_coordinates[-1][0]
                        #     vec1_j=cones[j][1]-right_cone_coordinates[-1][1]
                        #     vec2_i=left_cone_coordinates[-1][0]-right_cone_coordinates[-1][0]
                        #     vec2_j=left_cone_coordinates[-1][1]-right_cone_coordinates[-1][1]
                        # elif(distance(cones[j],left_cone_coordinates[-1])<1.4):
                        #     z=False
                        #     cones[j][0]=left_cone_coordinates[-1][0]
                        #     cones[j][1]=left_cone_coordinates[-1][1]
                        #     vec1_i=cones[i][0]-left_cone_coordinates[-1][0]
                        #     vec1_j=cones[i][1]-left_cone_coordinates[-1][1]
                        #     vec2_i=right_cone_coordinates[-1][0]-left_cone_coordinates[-1][0]
                        #     vec2_j=right_cone_coordinates[-1][1]-left_cone_coordinates[-1][1]
                        elif(distance(cones[j],right_cone_coordinates[-1])<1.4):
                            if(distance(cones[j], left_cone_coordinates[-1])<1.0):
                                continue
                            z=False
                            cones[j][0]=right_cone_coordinates[-1][0]
                            cones[j][1]=right_cone_coordinates[-1][1]
                            vec1_i=cones[i][0]-right_cone_coordinates[-1][0]
                            vec1_j=cones[i][1]-right_cone_coordinates[-1][1]
                            vec2_i=left_cone_coordinates[-1][0]-right_cone_coordinates[-1][0]
                            vec2_j=left_cone_coordinates[-1][1]-right_cone_coordinates[-1][1]
                    if(z==False):
                        dot=(vec1_i*vec2_i)+(vec1_j*vec2_j)
                        mag1=math.sqrt((vec1_i**2)+(vec1_j**2))
                        mag2=math.sqrt((vec2_i**2)+(vec2_j**2))
                        y=dot/(mag1*mag2)
                        # print("y=", y)
                        if(y>1.0):
                            y=1.0
                        angle=math.acos(y)
                        angle=math.degrees(angle)
            

                    left_angle=1
                    if(len(left_cone_coordinates)>=2 and distance(left_cone_coordinates[-1],cones[j])>0.5):
                        # left_angle=180
                        k=-2
                        while(distance(left_cone_coordinates[k],left_cone_coordinates[-1])<0.3):
                            k-=1
                            if(abs(k)>len(left_cone_coordinates)):
                                k=0
                                break
                        if(k!=0):
                            v1_i=cones[j][0]-left_cone_coordinates[-1][0]
                            v1_j=cones[j][1]-left_cone_coordinates[-1][1]
                            v2_i=left_cone_coordinates[-1][0]-left_cone_coordinates[k][0]
                            v2_j=left_cone_coordinates[-1][1]-left_cone_coordinates[k][1]
                            dot=(v1_i*v2_i)+(v1_j*v2_j)
                            mag1=math.sqrt((v1_i**2)+(v1_j**2))
                            mag2=math.sqrt((v2_i**2)+(v2_j**2))
                            # print("mag1=",mag1," mag2=",mag2," dot=",dot," theta=",dot/(mag1*mag2))
                
                            y3=dot/(mag1*mag2)
                            # print("y3=", y3)
                            if(y3>1.0):
                                y3=1.0
                            if(y3<-1.0):
                                y3=-1.0
                            left_angle=math.acos(y3)
                            left_angle=math.degrees(left_angle)

                    right_angle=1
                    if(len(right_cone_coordinates)>=2 and distance(right_cone_coordinates[-1],cones[i])>0.5):
                        # right_angle=180
                        k=-2
                        while(distance(right_cone_coordinates[k],right_cone_coordinates[-1])<0.3):
                            k-=1
                            if(abs(k)>len(right_cone_coordinates)):
                                k=0
                                break
                        if(k!=0):
                            v1_i=cones[i][0]-right_cone_coordinates[-1][0]
                            v1_j=cones[i][1]-right_cone_coordinates[-1][1]
                            v2_i=right_cone_coordinates[-1][0]-right_cone_coordinates[k][0]
                            v2_j=right_cone_coordinates[-1][1]-right_cone_coordinates[k][1]
                            dot=(v1_i*v2_i)+(v1_j*v2_j)
                            mag1=math.sqrt((v1_i**2)+(v1_j**2))
                            mag2=math.sqrt((v2_i**2)+(v2_j**2))
                            # print("mag1=",mag1," mag2=",mag2," dot=",dot," theta=",dot/(mag1*mag2))
                            
                            y2=dot/(mag1*mag2)
                            # print("y2=", y2)
                            if(y2>1.0):
                                y2=1.0
                            if(y2<-1.0):
                                y2=-1.0
                            right_angle=math.acos(y2)
                            right_angle=math.degrees(right_angle)
                    ang=0
                    if(len(left_cone_coordinates)>1):
                        prev_dir_i=reference[0]-((left_cone_coordinates[-2][0]+right_cone_coordinates[-2][0])/2)
                        prev_dir_j=reference[1]-((left_cone_coordinates[-2][1]+right_cone_coordinates[-2][1])/2)
                        new_dir_i=((cones[i][0]+cones[j][0])/2)-reference[0]
                        new_dir_j=((cones[i][1]+cones[j][1])/2)-reference[1]
                        m1=math.sqrt((prev_dir_i**2)+(prev_dir_j**2))
                        m2=math.sqrt((new_dir_i**2)+(new_dir_j**2))
                        do=(prev_dir_i*new_dir_i)+(prev_dir_j*new_dir_j)
                        
                        y1=do/(m1*m2)
                        # print("y1=", y1)
                        if(y1>1.0):
                            y1=1.0
                        ang=math.acos(y1)
                        ang=math.degrees(ang)

                    u=True
                    if(len(left_cone_coordinates)>1):
                        # print("u_left=",distance(left_cone_coordinates[-1],cones[j]),"u_right=",distance(right_cone_coordinates[-1],cones[i]))
                        u=False
                        if(distance(left_cone_coordinates[-1],cones[j])<7.0):
                            if(distance(right_cone_coordinates[-1],cones[i])<7.0):
                                
                                u=True

                    
                    mid_x=(cones[i][0]+cones[j][0])/2
                    mid_y=(cones[i][1]+cones[j][1])/2
                    mid=[mid_x,mid_y]
                    minimum=distance(reference,mid)
                    print("minimum distance=",minimum)
                    print("angle=",angle,"left_angle=",left_angle," right_angle=",right_angle," ang=",ang," z=",z," u=",u," distance=",distance(cones[i],cones[j]))
                    if ((ang<85) and (left_angle<45) and (right_angle<80) and (z  or (angle<60 and angle>15)) and True and distance(cones[i],cones[j])<8.0):
                        print("inside 1")
                        if((minimum<min_distance) and (minimum>0.8)):
                            print("inside 2")
                            min_distance=minimum
                            l=[0,0]
                            r=[0,0]
                            l[0]=cones[i][0]
                            l[1]=cones[i][1]
                            r[0]=cones[j][0]
                            r[1]=cones[j][1]
                            min_left_coordinate=l
                            min_right_coordinate=r
                    
                    if(k==1):
                        temp = cones[i]
                        cones[i] = cones[j]
                        cones[j] = temp
        print("min left coordinate=",min_left_coordinate)
        print("min right coordinate=",min_right_coordinate)

        print("leftList=", left_cone_coordinates)
        print("rightList=", right_cone_coordinates)


        if ((len(left_cone_coordinates)==0) and (distance(reference,car_coordinate)<1)):
            left_cone_coordinates.append(min_left_coordinate)
            right_cone_coordinates.append(min_right_coordinate)

            #------------publish---------------
            conepair=Arrofarr()
            leftcone = Arr()
            rightcone = Arr()
            leftcone.data = [float(min_left_coordinate[0]), float(min_left_coordinate[1])]
            rightcone.data = [float(min_right_coordinate[0]), float(min_right_coordinate[1])]
            conepair.data = [leftcone, rightcone]
            self.conePair_pub_.publish(conepair)
            
            print("published: ", leftcone.data, rightcone.data)
            # print("leftList=", left_cone_coordinates)
            # print("rightList=", right_cone_coordinates)
            min_distance=10000
            min_left_coordinate=[-1000,-1000]
            min_right_coordinate=[-1000,-1000]
            
        else:
            A1=left_cone_coordinates[-1][1]-right_cone_coordinates[-1][1]
            B1=right_cone_coordinates[-1][0]-left_cone_coordinates[-1][0]
            C1=(left_cone_coordinates[-1][0]*right_cone_coordinates[-1][1])-(right_cone_coordinates[-1][0]*left_cone_coordinates[-1][1])
            # print("A1=",A1," B1=",B1," c1=",C1)
            d=(abs((A1*car_coordinate[0])+(B1*car_coordinate[1]+C1))/math.sqrt((A1**2)+(B1**2)))
            # print("d", d)
            if(d<0.3):
                left_cone_coordinates.append(min_left_coordinate)
                right_cone_coordinates.append(min_right_coordinate)

                #------------publish---------------
                conepair=Arrofarr()
                leftcone = Arr()
                rightcone = Arr()
                leftcone.data = [float(min_left_coordinate[0]), float(min_left_coordinate[1])]
                rightcone.data = [float(min_right_coordinate[0]), float(min_right_coordinate[1])]
                conepair.data = [leftcone, rightcone]
                self.conePair_pub_.publish(conepair)

                print("published: ", leftcone.data, rightcone.data)
                # print("leftList=", left_cone_coordinates)
                # print("rightList=", right_cone_coordinates)
                min_distance=10000
                min_left_coordinate=[-1000,-1000]
                min_right_coordinate=[-1000,-1000]



def main(args=None):
    rclpy.init(args=args)
    node = midPointOriginal()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()