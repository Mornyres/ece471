# I - Import and Initialize
import pygame,pygame.locals
import sprite_module
import random
import os
import numpy as np

def main():
      
    pygame.init()
    pygame.display.set_caption("")
     
    # Entities
    #Create Background
    # TODO: modular background with geometry
    background = pygame.image.load('models/house.png')
    
    # make 'playing' area correspond to image dimensions
    HUDHEIGHT = 50 # matter of preference : bar for clear display of info
    WIDTH = background.get_rect().size[0]
    HEIGHT = background.get_rect().size[1] + HUDHEIGHT
    screen = pygame.display.set_mode((WIDTH, HEIGHT))    
    

    #background.scroll(0,HUDHEIGHT)
    
    background=background.convert()
    screen.blit(background, (0, 0+HUDHEIGHT))
    

    player=sprite_module.Player(screen)
    
    #Create the tags
    # define number of tags needed and optimize placement
    # TODO
    
    
    #loads tag images
    tag_img = pygame.image.load('models/tag.png')
    
    # TODO
    # size, color, position, content, alpha
    infomodules = 0

    infomodules = infomodules + 1 
    time_text=sprite_module.Text\
            (18,(255,255,0),(WIDTH-(WIDTH/infomodules)+200,HUDHEIGHT/2),\
            '0,0','Elapsed time:\t\t%sm %ss',\
            255)

    infomodules = infomodules + 1 
    position_text=sprite_module.Text\
            (18,(255,255,0),(WIDTH-(WIDTH/infomodules), HUDHEIGHT/2),\
            '0,0','[%s, %s]',\
            255)

    #Create sprite groups, sprites can be added to groups at any time
    tagGroup=pygame.sprite.Group()
    signal_img=pygame.sprite.Group()
    signal_hitbox=pygame.sprite.Group()
    
    #add sprite groups to be updated in the following order
    allSprites = pygame.sprite.OrderedUpdates\
    (player,tagGroup,signal_img,signal_hitbox,time_text,position_text)
     
    clock = pygame.time.Clock()
    elapsedTime = 0
    
    keepGoing = True
    while keepGoing:
        # the clock is updated for a max frame rate of 40 (recommended by
        # pygame docs)
        clock.tick(40)
        
        keystate = pygame.key.get_pressed()
        if keystate[pygame.locals.K_w]: 
            if player.rect.top > HUDHEIGHT:
                player.go_up(screen)
            else:
                None
        if keystate[pygame.locals.K_a]:
            player.go_left(screen)
        
        if keystate[pygame.locals.K_s]:
            player.go_down(screen)
                
        if keystate[pygame.locals.K_d]:
            player.go_right(screen)         

        # Events
        for event in pygame.event.get():
            
            #If the user closes the windows keepGoing is set to false and loop is exited
            if event.type == pygame.QUIT:
                keepGoing = False
            
            # possibly use click for manual pulse?
            elif event.type == pygame.MOUSEBUTTONDOWN:
                keepGoing = False
            elif event.type == pygame.MOUSEBUTTONUP:
                keepGoing = False

                                
        #Collision
        #Collision between signal  and tag
        c=pygame.sprite.groupcollide(signal_hitbox,tagGroup,True,False)
        v=pygame.sprite.groupcollide(signal_img,tagGroup,True,False)
        
        #Rotate player towards mouse            
        player.rotate(pygame.mouse.get_pos())
        
        # display,
        # color,
        # xy width height
        pygame.draw.rect(screen,\
                (0,0,255),\
                (0,0,WIDTH,HUDHEIGHT),\
                0)

        # Display elapsed time
        elapsedTime = elapsedTime + clock.get_time()
        time_text.set_variable(1,str(\
                np.round(((elapsedTime/1000) % 60),1,None)\
                ))
        time_text.set_variable(0,str(\
                np.int(np.floor((elapsedTime/1000) / 60))\
                ))

        # Display player position
        position_text.set_variable(0,\
                player.rect.left)
        position_text.set_variable(1,\
                (player.rect.top-HUDHEIGHT))


        # Refresh screen
        screen.blit(background,(0,0+HUDHEIGHT))
        allSprites.update()
        allSprites.draw(screen)
         
        pygame.display.flip()
     
    # Close the game window
    pygame.quit()    
     
         
# Call the main function
main()
