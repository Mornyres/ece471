import pygame,math,os,random
                
class Text(pygame.sprite.Sprite):
    def __init__(self,size,color,position,variables,message,alpha):
        pygame.sprite.Sprite.__init__(self)
        
        self.__font=pygame.font.Font("fonts/fixedwidth.ttf",size)
        self.__color=color
        self.__position=position
        
        if variables:
            self.__variables=variables.split(',') #splits variables into a list
            
        self.__message=message
        self.__m=''
        self.__alpha=alpha
        
    def set_variable(self,index,value):
        '''accepts an index and a value to set the variable at that index to'''
        self.__variables[index]=value
        
    def set_alpha(self,alpha):
        self.__alpha=alpha
        
    def update(self):
        '''This method will be called automatically to display
        the text at the set position.'''
        if self.__variables:
            #string formating using zip to create a tuple from list
            self.__m = self.__message %\
                    (list(zip(*[iter(self.__variables)]*len(self.__variables)))[0])
        else:
            self.__m=self.__message
            
        self.image = self.__font.render(self.__m, 1, self.__color)
        self.image.set_alpha(self.__alpha)
        self.rect = self.image.get_rect()
        self.rect.center = (self.__position)
        
class tag(pygame.sprite.Sprite):
    def __init__(self,screen,speed,damage,hp,attack_speed,value,image,zombie_type,player_pos):
        pygame.sprite.Sprite.__init__(self)
        #Assign screen to a variable
        self.__tagid=tagid
        
        #Assign speed to a variable
        self.__speed=speed
        self.__default_speed=speed
        
        #Assign the bullet damage to a variable
        self.__damage=damage
        
        #Assign hp to a variable
        self.__hp=hp
        
        #Assign attack speed to a variable
        self.__attack_speed=attack_speed
        
        #Assign value to a variable
        self.__value=value
        
        #Assign the wave type to a varible
        self.__zombie_type=zombie_type
        
        #Set variables for move
        self.__move=True
        
        #Set a counter for attack speed
        self.__count=(attack_speed-1)
        #Set up variables for slow
        self.__slow=False
        self.__slow_counter=0
            
        self.image=image
        self.image.convert_alpha()
        self.__saved_image=self.image
        self.rect = self.image.get_rect() 
        
        #Assign the enemy position
        self.spawn()
        
        
    def spawn(self):
        self.__spawn=random.randint(1,3)
        #Left
        if self.__spawn==1:
            self.__x=random.randrange(0,-300,-30)
            self.__y=random.randint(0,self.__screen.get_height()-100)
        #Right
        elif self.__spawn==2:
            self.__x=random.randint(self.__screen.get_width(),self.__screen.get_width()+300)
            self.__y=random.randint(0,self.__screen.get_height()-100)  
        #Bottom
        else:
            self.__x=random.randint(0,self.__screen.get_width())
            self.__y=random.randint(self.__screen.get_height(),self.__screen.get_height()+300)
            
        self.rect.center=(self.__x,self.__y)
     
class Player(pygame.sprite.Sprite):
    def __init__(self,screen):
        pygame.sprite.Sprite.__init__(self)
        
        #Create list of images
        self.__list = []
        for file in os.listdir('models/player/'):
            self.__list.append(pygame.image.load('./models/player/'+file))
            
        #Set original image
        self.image = self.__list[0]
        self.image = self.image.convert_alpha()
        self.rect = self.image.get_rect()    
        
        #Set saved image
        self.__saved_image = self.image
        
        # Set the rect attribute for our player sprite
        self.rect.left = 0
        self.rect.top= 200
        self.__speed = 2
        
        #set the angle value
        self.__angle=0
        
    def go_left(self,screen):
        if self.rect.left < 0:
            None
        else:
            self.rect.left-=self.__speed
    
    def go_right(self,screen):
        if self.rect.right > screen.get_width():
            None
        else:
            self.rect.right+=self.__speed

    def go_up(self,screen):
        if self.rect.top < 0:
            None
        else:
            self.rect.top-=self.__speed
    
    def go_down(self,screen):
        if self.rect.bottom > screen.get_height():
            None
        else:
            self.rect.bottom+=self.__speed
    
    def get_angle(self):
        return self.__angle
    
    def change_image(self,weapon):
        #Set original image
        self.image = self.__list[weapon]
        self.image = self.image.convert_alpha()
        
        #Set saved image
        self.__saved_image = self.image
    def rotate(self,mouse_pos):
        self.__angle = math.degrees(math.atan2\
              (self.rect.centerx-mouse_pos[0], self.rect.centery-mouse_pos[1]))
        
        self.image=pygame.transform.rotate\
            (self.__saved_image, self.__angle)
        
        self.rect = self.image.get_rect(center=self.rect.center)

