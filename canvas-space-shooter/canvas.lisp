(defpackage :canvas
  (:use :common-lisp
        :cl-who
        :parenscript))

(in-package :canvas)

(defvar *screen-size* (list 500 500))
(defvar *number-of-enemies* 100)
(defvar *chance-of-enemy-fire* 0.001)        ;per update chance of firing

(defmacro/ps draw-and-update (designator)
  `(progn
     (chain ,designator (update))
     (chain ,designator (draw))))

(defmacro/ps draw-and-update-list (designator)
  (let ((tmp (ps-gensym)))
    `(progn
       (dotimes (i (@ ,designator length))
         (setf cur-index i)
         (var ,tmp (elt ,designator i))
         (if ,tmp
             (draw-and-update ,tmp))))))

(defmacro/ps clear-nil-from-array (array)
  (let ((new-array (ps-gensym)))
    `(progn
       (var ,new-array (chain ,array (filter (lambda (obj) (if obj (return true) (return false))))))
       (setf ,array ,new-array))))

(defun gen-main-js-static ()
  (ps (var ctx)
      (var objects)
      (var cur-index)

      (defun draw-objects ()
        ;; draw player
        (let ((player (@ objects player)))
          (if player

              (draw-and-update player)))
        ;; draw missiles
        (draw-and-update-list (@ objects player-missiles))
        (draw-and-update-list (@ objects enemy-missiles))
        ;; draw enemies
        (draw-and-update-list (@ objects enemies)))
	  
      (defun clear-objects ()
        ;; add other arrays here that need to be cleared
        (clear-nil-from-array (@ objects player-missiles))
        (clear-nil-from-array (@ objects enemy-missiles))
        (clear-nil-from-array (@ objects enemies)))

      (defun draw ()
        (chain ctx (clear-rect 0 0 (@ size 0) (@ size 1)))
        (draw-objects))                                       

      (defun test-collision (c1 c2)
        (let ((s1 (- (@ c1 x) (@ c2 x)))
              (s2 (- (@ c1 y) (@ c2 y))))
          (if (< (chain *math (sqrt (+ (* s1 s1) (* s2 s2))))
                 (+ (@ c1 size) (@ c2 size)))
              (return true)
              (return false))))

      (defun *circle (x y s dx dy)
        (let ((lbx s)
              (ubx (- (@ size 0) s))
              (lby s)
              (uby (- (@ size 1) s)))
          (setf (@ this x) x
                (@ this y) y
                (@ this fill-style) "#0000ff"

                (@ this size) s
                (@ this dx) dx
                (@ this dy) dy
                (@ this bounds) ([] lbx lby ubx uby)
                (@ this draw) (lambda ()
                                (with-slots (x y size fill-style) this
                                  (chain ctx (begin-path))
                                  (setf (@ ctx fill-style) fill-style)
                                  (chain ctx (arc x y size 0 (* 2 (@ *math +pi+)) true))
                                  (chain ctx (close-path))
                                  (chain ctx (fill)))))))

      (defun *player (x y)
        (setf (@ this __proto__) (new (*circle x y 10 0 0))
              (@ this fill-style) "#0000ff"
              (@ this update) (lambda ()
                                ;; test for collisions
                                (dolist (missile (@ objects enemy-missiles))
                                  (if missile
                                      (if (test-collision this missile)
                                          (setf (@ objects player) nil)))))))


      (defun *player-missile (x y)
        (setf (@ this __proto__) (new (*circle x y 2 0 -2))
              (@ this fill-style) "#0000ff"

              (@ this update) (lambda ()
                                (with-slots (y dy bounds) this
                                  (if (< y (elt bounds 1))
                                      (setf (elt (@ objects player-missiles) cur-index) nil))
                                  (incf y dy)))))                               

      (defun *enemy-missile (x y)
        (setf (@ this __proto__) (new (*circle x y 2 0 2))
              (@ this fill-style) "#ff0000"

              (@ this update) (lambda ()
                                (with-slots (y dy bounds) this
                                  (if (> y (elt bounds 3))
                                      (setf (elt (@ objects enemy-missiles) cur-index) nil))
                                  (incf y dy)))))
	  
      (defun on-mouse-click (evt)
        (when (@ objects player)

          (var missile (new (*player-missile (@ objects player x)
                                             (@ objects player y))))
          (setf (@ objects player-missiles)
                (append (@ objects player-missiles) missile))))

      (chain ($ document) (ready init))
      (chain ($ document) (mousemove on-mouse-move))
      (chain ($ document) (mousedown on-mouse-click))))

(defun gen-main-js-dynamic ()
  (let ((enemy-positions (append (list 'list) (loop for i below *number-of-enemies* collect
												   `(list ,(* (random 1.0) (first *screen-size*))
														  ,(* (random 1.0) (- (second *screen-size*) (* 2 (/ (second *screen-size*) 3))))
														  ,(- (* 2 (random 1.0)) 1)
														  ,(- (* 2 (random 1.0)) 1))))))
    (ps* `(progn
            (defun init ()
              (let ((c (new (*player ,(/ (first *screen-size*) 2) ,(- (second *screen-size*) 10)))))

                (setf ctx (chain ($ :#canvas) 0 (get-context :2d))
                      objects (create player c player-missiles (make-array) enemy-missiles (make-array) enemies (make-array))))
              (var enemy-positions ,enemy-positions)
              (dotimes (i ,*number-of-enemies*)

                (let ((enemy (new (*enemy (@ (elt enemy-positions i) 0)
                                          (@ (elt enemy-positions i) 1)
                                          (@ (elt enemy-positions i) 2)
                                          (@ (elt enemy-positions i) 3)))))
                  (setf (elt (@ objects enemies) i) enemy)))
              (set-interval clear-objects 100)
              (return (set-interval draw 10)))

            (defun *enemy (x y dx dy)
              (setf (@ this __proto__) (new (*circle x y 4 dx dy))
                    (@ this bounds 3) (- (@ this bounds 3) (* 2 (/ (@ this bounds 3) 3)))
                    (@ this fill-style) "#008888"

                    (@ this update) (lambda ()
                                      (with-slots (x y dx dy bounds) this
                                        (if (or (> x (elt bounds 2))
                                                (< x (elt bounds 0)))
                                            (setf dx (- dx)))
                                        (if (or (> y (elt bounds 3))
                                                (< y (elt bounds 1)))
                                            (setf dy (- dy)))
                                        (incf x dx)
                                        (incf y dy))
                                      ;; test for collisions
                                      (dolist (missile (@ objects player-missiles))
                                        (if missile
                                            (if (test-collision this missile)
                                                (setf (elt (@ objects enemies) cur-index) nil))))
                                      ;; randomly fire missile
                                      (when (< (chain *math (random)) ,*chance-of-enemy-fire*)
                                        (var missile (new (*enemy-missile (@ this x)
                                                                          (@ this y))))
                                        (setf (@ objects enemy-missiles)
                                              (append (@ objects enemy-missiles) missile))))))

			
            (defun on-mouse-move (evt)
              (when (@ objects player)
				(var canv (elt ($ :#canvas) 0))
                (var amount ,(round (/ (first *screen-size*) 3)))
                (var left (@ canv offset-left))
                (var top (+ (@ canv offset-top ) (* 2 amount)))
                (var right (+ left (@ canv offset-width)))
                (var bottom (+ top amount))
                (with-slots (page-x page-y) evt
                  (if (and (> page-x left)
                           (< page-x right))
                      (setf (@ objects player x) (- page-x (@ canv offset-left))))
                  (if (and (> page-y top)
                           (< page-y bottom))
                      (setf (@ objects player y) (- page-y (@ canv offset-top)))))))

            (var size ([] ,(first *screen-size*) ,(second *screen-size*)))))))


(with-open-file (out "test.js" :direction :output :if-exists :supersede)
  (write-string (gen-main-js-static) out)
  (write-string (gen-main-js-dynamic) out))

(with-open-file (out "test.html" :direction :output :if-exists :supersede)
  (with-html-output (out)
    (htm :html
         (:head
          (:script :type "text/javascript" :src "jquery-1.3.2.min.js")
          (:script :type "text/javascript" :src "test.js")
          (:title "Testing"))
         (:body
          (:audio :id "sound1" :src "tada.wav" "Unsupported Audio")
          (:canvas :id "canvas" :width (first *screen-size*) :height (second *screen-size*))))))