(use c-wrapper)
(use egauche.e)
(use file.util)
(e-load (build-path (sys-getenv "HOME") "c/experiment/egauche/src/ew.h"))

(print ">>")

(define *w* #f)

(define *input-window* #f)
(define *handlers* NULL)

(define (show)
  (define (add-handler! ev cb)
    (update! *handlers*
      (^x (eina_list_append
            x (ecore_event_handler_add ev cb (make-null-ptr))))))
  (define (make-input-window _z)
    (rlet1 w (ecore_x_window_new 0 20 20 200 200)
           (ecore_x_window_show w)
           ;;(e_grab_input_get w 0 w)
           ))

  (set! *input-window* (make-input-window 'ignore))
  (add-handler! ECORE_EVENT_KEY_DOWN keydown))

(define (keydown _data _type event)
  ECORE_CALLBACK_PASS_ON)

(use t.interactive)

(eop (show))

(print "<<")
