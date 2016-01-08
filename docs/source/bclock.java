// The Berlin clock (R) as a Java (1.0) applet
//
// 19-07-98 -- jd --

import java.awt.*;
import java.applet.*;
import java.util.Date;

public class bclock extends java.applet.Applet implements Runnable {

    Thread bclockThread;
    Date startDate;

    public void init() {
        resize(242,180); // fixed size, at first... doesn't work...
    }
    public void start() {
        if (bclockThread == null) {
            bclockThread = new Thread(this, "bclock");
            bclockThread.start();
        }
    }
    public void run() {
        while (bclockThread != null) {
            repaint();
            try {
                bclockThread.sleep(1000);
            } catch (InterruptedException e){
            }
        }
    }
    public void update(Graphics g) {
        Date now = new Date();
        int min,stu,i;

        if (now.getSeconds() % 2 > 0) { g.setColor(Color.black); }
        else { g.setColor(Color.yellow); }
        g.fillOval(100,4,40,40);
        stu = now.getHours();
        min = now.getMinutes();
        if (min == 0 && stu == 0) { stu = 24; }
        for (i = 0; i < 4; i++) {
            if (stu / ((i+1) * 5) > 0) { g.setColor(Color.red); }
            else { g.setColor(Color.black); }
            g.fillRoundRect(i*60+2,46,58,30,4,4);
        }
        for (i = 0; i < 4; i++) {
            if (stu % 5 > i) { g.setColor(Color.red); }
            else { g.setColor(Color.black); }
            g.fillRoundRect(i*60+2,78,58,30,4,4);
        }
        for (i = 0; i < 11; i++) {
            if (min / ((i+1) * 5) > 0) {
                if (i == 2 || i == 5 || i == 8) { g.setColor(Color.red); }
                else { g.setColor(Color.yellow); }
            }
            else { g.setColor(Color.black); }
            g.fillRoundRect(i*20+10,110,18,30,4,4);
        }
        for (i = 0; i < 4; i++) {
            if (min % 5 > i) { g.setColor(Color.yellow); }
            else { g.setColor(Color.black); }
            g.fillRoundRect(i*60+2,142,58,30,4,4);
        }
        
    }
    public void paint(Graphics g) {
	update(g); // makes it flicker free
    }
    public void stop() {
        bclockThread.stop();
        bclockThread = null;
    }
}
