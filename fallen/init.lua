require('libblacklua')

dofile('load_images.lua')
dofile('load_sounds.lua')
dofile('global_callbacks.lua')
dofile('player_fire.lua')
dofile('wave.lua')
dofile('mob_drop.lua')

bl.manager.set_ticks_per_sec(60)

intro_state = bl.state.new(0)
main_state = bl.state.new(1)

bl.manager.switch_state(intro_state)
GlobalState = "INTRO"
IntroState = "START"

keystates = {left=0, right=0, up=0, down=0, space=0}

global = {obj = bl.object.new()}
bl.state.push(main_state, global.obj)

intro_obj = {obj = bl.object.new()}
bl.state.push(intro_state, intro_obj.obj)
bl.object.update_append(intro_obj.obj, "exit_update")
bl.object.update_append(intro_obj.obj, "event_tosser")
bl.object.update_append(intro_obj.obj, "start_button_update")
bl.object.render_append(intro_obj.obj, "start_button_render")
bl.object.update_append(intro_obj.obj, "exit_button_update")
bl.object.render_append(intro_obj.obj, "exit_button_render")
bl.object.render_append(intro_obj.obj, "title_render")

player = {obj = bl.object.new(),
          anim_sequence = "player-top-sequence",
          x = 800/2,
          y = (600-32),
          deltaX = 0,
          deltaY = 0,
          dampingCoeff = 0.1,
          incAmount = 1,
          maxMove = 10,
          minY = 500,
          energy = 500,
          energy_max = 500,
          fire_cooldown = 0,
          fire_cooldown_reset = 10,
          image = nil,
          score = 0,
          lives = 3,
          wave = 1,
          state = "NORMAL",
          death_counter = 0,
       }

function player_get_poly ()
   return {player.x+1, player.y+24,
           player.x+30, player.y+24,
           player.x+15, player.y+9}
end

bl.state.push(main_state, player.obj)

bl.object.render_append(player.obj, "draw_player")

bl.object.update_append(global.obj, "gc_bullets")
bl.object.update_append(global.obj, "gc_drops")
bl.object.update_append(global.obj, "gc_mobs")
--bl.object.render_append(global.obj, "draw_fps")
bl.object.render_append(global.obj, "draw_score")
bl.object.render_append(global.obj, "draw_wave")
bl.object.render_append(global.obj, "draw_lives")
bl.object.render_append(global.obj, "draw_energy")

function player_attempt_move (type)
   if type == 'left' then
      player.deltaX = player.deltaX - player.incAmount
      if player.deltaX < -player.maxMove then player.deltaX = -player.maxMove end
   elseif type == 'right' then
      player.deltaX = player.deltaX + player.incAmount
      if player.deltaX > player.maxMove then player.deltaX = player.maxMove end
   elseif type == 'up' then
      player.deltaY = player.deltaY - player.incAmount
      if player.deltaY < -player.maxMove then player.deltaY = -player.maxMove end
   elseif type == 'down' then
      player.deltaY = player.deltaY + player.incAmount
      if player.deltaY > player.maxMove then player.deltaY = player.maxMove end
   end
end

function energy ()
   if count_live_mobs() <= 4 then
      -- give the player a break when mobs get low
      player.energy = player.energy + 1
   else
      player.energy = player.energy + 0.25
   end

   if player.energy >= player.energy_max then
      player.energy = player.energy_max
   end
end

function player_initial_mover (gm, obj, ticks)
   --if ticks % 10 == 0 then
   inertial_move()
   --end

   player.image = bl.image.set_get(player.anim_sequence, ticks)
end

function inertial_move ()
   if player.deltaX ~= 0 then
      newX = player.x + player.deltaX
      if newX <= 0 then newX = 0; player.deltaX = 0 end
      if newX >= (800-32) then newX = (800-32); player.deltaX = 0 end
      player.x, player.y = newX, player.y
      
      if player.deltaX > 0 then
         player.deltaX = player.deltaX - player.dampingCoeff
         if player.deltaX < 0 then player.deltaX = 0 end
      elseif player.deltaX < 0 then
         player.deltaX = player.deltaX + player.dampingCoeff
         if player.deltaX > 0 then player.deltaX = 0 end
      end
   else newX = player.x end

   if player.deltaY ~= 0 then
      newY = player.y + player.deltaY
      if newY <= player.minY then newY = player.minY; player.deltaY = 0 end
      if newY >= (600-32) then newY = (600-32); player.deltaY = 0 end
      
      if player.deltaY > 0 then
         player.deltaY = player.deltaY - player.dampingCoeff
         if player.deltaY < 0 then player.deltaY = 0 end
      elseif player.deltaY < 0 then
         player.deltaY = player.deltaY + player.dampingCoeff
         if player.deltaY > 0 then player.deltaY = 0 end
      end
   else newY = player.y end

   player.x, player.y = newX, newY
end



function draw_player (gm, obj, int)
   if player.image then
      bl.draw.image(player.image, player.x, player.y)
   end
end

function draw_fps (gm, obj, int)
   local text = "FPS: " .. bl.info.fps()
   bl.draw.text("VeraMono.ttf", 20, text, 10, 10, 0, 250, 0)
end

-- text x, y, rect x, rect y, rect width, rect height
draw_energy_position = {20, 10, 110, 15, 680, 18}
function draw_energy (gm, obj, int)
   local text = "Energy:"
   
   local energy = player.energy / player.energy_max
   if energy <= 0 then
      energy = 0
   end
   if energy >= 0.25 then
      color = {0, 0, 200}
   elseif energy >= 0.15 then
      color = {150, 150, 0}
   else
      color = {200, 0, 0}
   end

   bl.draw.text("VeraMono.ttf", 20, text, draw_energy_position[1], draw_energy_position[2], 0, 0, 250)
   bl.draw.rect(draw_energy_position[3], draw_energy_position[4], draw_energy_position[5],
                draw_energy_position[6], 0, 0, 100)
   bl.draw.rect(draw_energy_position[3], draw_energy_position[4], draw_energy_position[5] * energy,
                draw_energy_position[6], color[1], color[2], color[3])
end

-- text x, y
draw_score_position = {300, 40}
function draw_score (gm, obj, int)
   local text = "Score: " .. player.score
   bl.draw.text("VeraMono.ttf", 20, text, draw_score_position[1], draw_score_position[2], 0, 0, 250)
end

-- text x, y
draw_wave_position = {160, 40}
function draw_wave (gm, obj, int)
   local text = "Wave: " .. player.wave
   bl.draw.text("VeraMono.ttf", 20, text, draw_wave_position[1], draw_wave_position[2], 0, 0, 250)
end

-- text x, y
draw_lives_position = {20, 40}
function draw_lives (gm, obj, int)
   local text = "Lives: " .. player.lives
   bl.draw.text("VeraMono.ttf", 20, text, draw_lives_position[1], draw_lives_position[2], 0, 0, 250)
end

function hit_checker (gm, obj, ticks)
   if player.state == "NORMAL" then
      if player.energy <= 0 then
         death_checker_counter = ticks
         player.lives = player.lives - 1
         player.energy = player.energy_max
         wave_pause()
         fire_kill()
         drop_kill()
         player.state = "EXPLODING"
         player.death_counter = ticks
         player.anim_sequence = "player-explosion"
         bl.sound.play("player-die")
      end
   elseif player.state == "EXPLODING" then
      if ticks - player.death_counter >= 50*2 then
         player.state = "NORMAL"
         player.anim_sequence = "player-top-sequence"
         wave_play()
      end
   end      
end

-- text x, y
draw_final_score_position = {300, 280}
function draw_final_score (gm, obj, int)
   local text = "Final Score: " .. player.score
   bl.draw.text("VeraMono.ttf", 30, text, draw_final_score_position[1], draw_final_score_position[2], 0, 0, 250)
end

function draw_new_high_score (gm, obj, int)
   local text = "New High Score!"
   bl.draw.text("VeraMono.ttf", 30, text, draw_final_score_position[1], draw_final_score_position[2]+50, 0, 0, 250)
end

death_checker_counter = 0
death_started = false
function death_checker (gm, obj, ticks)
   if player.lives <= 0 then
      if not death_started then
         death_started = true
         bl.object.render_append(global.obj, "draw_final_score")
         local high = possible_write_high_score(player.score)
         if high then
            bl.object.render_append(global.obj, "draw_new_high_score")
         end
      end
      if ticks - death_checker_counter >= 60*4 then
         GlobalState = "INTRO"
         reset_game()
         bl.manager.switch_state(intro_state)
      end
   end      
end

function wave_checker (gm, obj, ticks)
   local count = 0
   for mob_obj, mob in pairs(WaveMobs) do
      count = count + 1
   end
   
   if count == 0 then
      -- move on to next wave
      player.wave = player.wave + 1
      build_wave(player.wave)
      player.energy_max = player.energy_max + 25
      player.energy = player.energy_max
      fire_kill()
      drop_kill()
   end
end

last_extra_life = 0
next_extra_life = 5000
function extra_life (gm, obj, ticks)
   if player.score - last_extra_life > next_extra_life then
      last_extra_life  = last_extra_life + 5000
      next_extra_life = next_extra_life + 5000
      player.lives = player.lives + 1
   end   
end

-- create wave of monsters
build_wave(player.wave)

function player_add_updates (gm, obj, ticks)
   bl.object.update_clear(player.obj)

   bl.object.update_append(player.obj, "exit_update")
   bl.object.update_append(player.obj, "event_tosser")
   bl.object.update_append(player.obj, "player_initial_mover")
   bl.object.update_append(player.obj, "player_mover")
   bl.object.update_append(player.obj, "energy")
   bl.object.update_append(player.obj, "fire_cooldown")
   bl.object.update_append(player.obj, "hit_checker")
   bl.object.update_append(player.obj, "death_checker")
   bl.object.update_append(player.obj, "wave_checker")
   bl.object.update_append(player.obj, "extra_life")
end

-- add player update handlers
player_add_updates()


start_button_image = nil
function start_button_update (gm, obj, ticks)
   if IntroState == "START" then
      start_button_image = bl.image.set_get("start-button-selected", ticks)
   else
      start_button_image = bl.image.set_get("start-button-default", ticks)
   end
end

function start_button_render (gm, obj, int)
   if start_button_image then
      bl.draw.image(start_button_image, 250, 350)
   end
end

exit_button_image = nil
function exit_button_update (gm, obj, ticks)
   if IntroState == "EXIT" then
      exit_button_image = bl.image.set_get("exit-button-selected", ticks)
   else
      exit_button_image = bl.image.set_get("exit-button-default", ticks)
   end
end

function exit_button_render (gm, obj, int)
   if exit_button_image then
      bl.draw.image(exit_button_image, 250, 390)
   end
end

function possible_write_high_score (score)
   if score > PreviousHighScore then
      PreviousHighScore = score
      local file = io.open("high_score.txt", "w")
      file:write(score)
      file:close()
      return true
   end
   
   return false
end

PreviousHighScore = 0
function read_high_score_from_file ()
   local file = io.open("high_score.txt", "r")
   PreviousHighScore = file:read("*n")
   file:close()
end

function get_high_score ()
   read_high_score_from_file()
   local value = PreviousHighScore
   local rev = string.reverse(value)
   local res_rev = ""
   for count = 1,string.len(value) do
      res_rev = res_rev .. string.sub(rev, count, count)
      if count % 3 == 0 and count ~= string.len(value) then
         res_rev  = res_rev .. ","
      end
   end
   return string.reverse(res_rev)
end

function title_render (gm, obj, int)
   local image = bl.image.set_get("title", 0)
   bl.draw.image(image, 100, 50)

   
   local text = {"Arcade space-shooting fun! Try your hand at defending",
                 "against the alien invaders as the waves get impossibly",
                 "hard and try to beat the high score."}
   for count = 1, #text do
      if count == 1 then
         bl.draw.text("VeraMono.ttf", 18, text[1], 125, 100, 0, 0, 250)
      else
         bl.draw.text("VeraMono.ttf", 18, text[count], 100, 100 + (count-1) * 20, 0, 0, 250)
      end
   end

   text = {"Avoid enemy fire and catch the blue energy bars as they",
           "drop. Your ship's energy depletes as you fire and as you take",
           "hits from the alien craft, then slowly recovers over time.",
           "When your energy goes to zero you lose a life. Gain more",
           "lives by hitting certain score limits."}
   for count = 1, #text do
      if count == 1 then
         bl.draw.text("VeraMono.ttf", 18, text[1], 125, 180, 0, 0, 250)
      else
         bl.draw.text("VeraMono.ttf", 18, text[count], 100, 180 + (count-1) * 20, 0, 0, 250)
      end
   end

   text = "High Score: " .. get_high_score()
   bl.draw.text("VeraMono.ttf", 24, text, 150, 540, 0, 200, 200)
end

function reset_game ()
   bl.object.render_remove(global.obj, "draw_final_score")
   bl.object.render_remove(global.obj, "draw_new_high_score")
   
   player.x = 800/2
   player.y = (600-32)
   player.deltaX = 0
   player.deltaY = 0
   player.dampingCoeff = 0.1
   player.incAmount = 1
   player.maxMove = 10
   player.minY = 500
   player.energy = 500
   player.energy_max = 500
   player.fire_cooldown = 0
   player.fire_cooldown_reset = 10
   player.image = nil
   player.score = 0
   player.lives = 3
   player.wave = 1
   player.state = "NORMAL"
   player.death_counter = 0

   build_wave(1)

   last_extra_life = 0
   next_extra_life = 5000
   
   death_started = false

   fire_kill()
   drop_kill()
   kill_mobs()
end