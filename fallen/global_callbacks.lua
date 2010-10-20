function exit_update (gm, obj, ticks)
   while true do
      local event = bl.event.peek()
      if not event then
         return
      end
      
      local type = bl.event.get_type(event)
      local key = bl.event.get_key(event)

      if GlobalState == "MAIN" then   
         if type == bl.event.type.SDL_QUIT then
            bl.manager.quit()
            bl.event.pop()
         elseif type == bl.event.type.SDL_KEYDOWN then
            if key == bl.event.key.SDLK_ESCAPE then
               GlobalState = "INTRO"
               reset_game()
               bl.manager.switch_state(intro_state)
            end
            if key == bl.event.key.SDLK_i then
               print('player position request: (' .. player.x .. ', ' .. player.y .. ')')
            end     
            
            if key == bl.event.key.SDLK_LEFT then keystates.left = 1
            elseif key == bl.event.key.SDLK_RIGHT then keystates.right = 1
            elseif key == bl.event.key.SDLK_UP then keystates.up = 1
            elseif key == bl.event.key.SDLK_DOWN then keystates.down = 1
            elseif key == bl.event.key.SDLK_SPACE then keystates.space = 1
            end
            
            bl.event.pop()
         elseif type == bl.event.type.SDL_KEYUP then
            if key == bl.event.key.SDLK_LEFT then keystates.left = 0
            elseif key == bl.event.key.SDLK_RIGHT then keystates.right = 0
            elseif key == bl.event.key.SDLK_UP then keystates.up = 0
            elseif key == bl.event.key.SDLK_DOWN then keystates.down = 0
            elseif key == bl.event.key.SDLK_SPACE then keystates.space = 0
            end
            
            bl.event.pop()
         else
            return
         end
      elseif GlobalState == "INTRO" then
         if type == bl.event.type.SDL_KEYDOWN then
            if key == bl.event.key.SDLK_RETURN then 
               if IntroState == "EXIT" then
                  bl.manager.quit()
               elseif IntroState == "START" then
                  GlobalState = "MAIN"
                  bl.manager.switch_state(main_state)
               end
            elseif key == bl.event.key.SDLK_DOWN then 
               IntroState = "EXIT"
            elseif key == bl.event.key.SDLK_UP then 
               IntroState = "START"
            end
            
            bl.event.pop()
         else
            return
         end
      end

   end
end

function event_tosser (gm, obj, ticks)
   while true do
      local event = bl.event.peek()
      if not event then break end
      --print('tossing event of type: ' .. tostring(bl.event.get_type(event)))
      bl.event.pop()
   end
end

function player_mover (gm, obj, ticks)
   if ticks % 3 == 0 then
      if keystates.left == 1 then player_attempt_move('left') end
      if keystates.right == 1 then player_attempt_move('right') end
      if keystates.up == 1 then player_attempt_move('up') end
      if keystates.down == 1 then player_attempt_move('down') end
   end

   if keystates.space == 1 then      
      player_fire()
   end
end