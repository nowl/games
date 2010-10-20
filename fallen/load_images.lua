bl.image.load("player-top", "tiles.png", 1, 1, 32, 32)
bl.image.load("player-top", "tiles.png", 1, 1, 32, 32)
bl.image.load("player-explode1", "tiles.png", 1+33, 1, 32, 32)
bl.image.load("player-explode2", "tiles.png", 1+2*33, 1, 32, 32)

bl.image.set_create("player-top-sequence")
bl.image.set_add("player-top-sequence", "player-top", 10)

bl.image.set_create("player-explosion")
bl.image.set_add("player-explosion", "player-explode1", 50)
bl.image.set_add("player-explosion", "player-explode2", 50)

bl.image.load("bullet1", "tiles.png", 1, 1+33, 32, 32)
bl.image.load("bullet2", "tiles.png", 1+33, 1+33, 32, 32)
bl.image.load("bullet3", "tiles.png", 1+33*2, 1+33, 32, 32)
bl.image.load("bullet4", "tiles.png", 1+33*3, 1+33, 32, 32)

bl.image.set_create("mob-bullet-seq")
bl.image.set_add("mob-bullet-seq", "bullet3", 5)
bl.image.set_add("mob-bullet-seq", "bullet4", 5)

bl.image.set_create("player-bullet-seq")
bl.image.set_add("player-bullet-seq", "bullet1", 5)
bl.image.set_add("player-bullet-seq", "bullet2", 5)

bl.image.load("red-mob1", "tiles.png", 1+33*3, 1, 32, 32)
bl.image.load("red-mob2", "tiles.png", 1+33*4, 1, 32, 32)
bl.image.load("red-mob3", "tiles.png", 1+33*5, 1, 32, 32)
bl.image.load("red-mob4", "tiles.png", 1+33*6, 1, 32, 32)
bl.image.load("green-mob1", "tiles.png", 1+33*7, 1, 32, 32)
bl.image.load("green-mob2", "tiles.png", 1+33*8, 1, 32, 32)
bl.image.load("green-mob3", "tiles.png", 1+33*9, 1, 32, 32)
bl.image.load("green-mob4", "tiles.png", 1+33*10, 1, 32, 32)

bl.image.set_create("red-mob")
bl.image.set_add("red-mob", "red-mob1", 100)

bl.image.set_create("red-mob-firing")
bl.image.set_add("red-mob-firing", "red-mob2", 100)

bl.image.set_create("red-mob-explosion")
bl.image.set_add("red-mob-explosion", "red-mob3", 20)
bl.image.set_add("red-mob-explosion", "red-mob4", 20)

bl.image.set_create("green-mob")
bl.image.set_add("green-mob", "green-mob1", 100)

bl.image.set_create("green-mob-firing")
bl.image.set_add("green-mob-firing", "green-mob2", 100)

bl.image.set_create("green-mob-explosion")
bl.image.set_add("green-mob-explosion", "green-mob3", 20)
bl.image.set_add("green-mob-explosion", "green-mob4", 20)

bl.image.load("drop1", "tiles.png", 1, 1+32*2, 32, 32)

bl.image.set_create("drop-seq1")
bl.image.set_add("drop-seq1", "drop1", 50)

bl.image.load("button1", "other.png", 2, 2, 75, 24)
bl.image.load("button2", "other.png", 1, 28, 75, 25)
bl.image.load("button3", "other.png", 2, 57, 60, 24)
bl.image.load("button4", "other.png", 1, 84, 60, 25)
bl.image.set_create("start-button-default")
bl.image.set_add("start-button-default", "button1", 10)
bl.image.set_create("start-button-selected")
bl.image.set_add("start-button-selected", "button2", 30)
bl.image.set_add("start-button-selected", "button1", 30)
bl.image.set_create("exit-button-default")
bl.image.set_add("exit-button-default", "button3", 10)
bl.image.set_create("exit-button-selected")
bl.image.set_add("exit-button-selected", "button4", 30)
bl.image.set_add("exit-button-selected", "button3", 30)

bl.image.load("title1", "other.png", 85, 2, 124, 24)
bl.image.set_create("title")
bl.image.set_add("title", "title1", 10)
