package com.misc.raspberrywebradio;

import java.io.IOException;
import java.net.Inet4Address;

import javax.jmdns.JmDNS;
import javax.jmdns.ServiceEvent;
import javax.jmdns.ServiceListener;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.util.Log;
import android.view.Window;
import android.webkit.WebView;
import android.webkit.WebViewClient;

public class WebRadio extends Activity {
    private class MyWebViewClient extends WebViewClient {
	@Override
	public boolean shouldOverrideUrlLoading(WebView view, String url) {
	    view.loadUrl(url);
	    return true;
	}
    }

    WebView browserView;

    android.net.wifi.WifiManager.MulticastLock lock;
    android.os.Handler handler = new android.os.Handler();

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
	super.onCreate(savedInstanceState);
	// Removes the title bar in the application
	requestWindowFeature(Window.FEATURE_NO_TITLE);
	setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
	setContentView(R.layout.main);

	// Creation of the Webview found in the XML Layout file
	browserView = (WebView) findViewById(R.id.webkit);

	// Override URL loading
	browserView.setWebViewClient(new MyWebViewClient());

	// Enable Javascripts
	browserView.getSettings().setJavaScriptEnabled(true);

	// Removes both vertical and horizontal scroll bars
	browserView.setVerticalScrollBarEnabled(false);
	browserView.setHorizontalScrollBarEnabled(false);

	// The website which is wrapped to the webview
	// browserView.loadUrl("http://raspberrypi.local/app.php");
	browserView
		.loadData(
			"<br><center><font color='#808080' family='Verdana'>Loading... Please wait...</font></center>",
			"text/html", "UTF8");

	handler.postDelayed(new Runnable() {
	    public void run() {
		setUp();
	    }
	}, 100);

    }

    /** Called when the activity is first created. */

    private String type = "_workstation._tcp.local.";
    private JmDNS jmdns = null;
    private ServiceListener listener = null;

    // private ServiceInfo serviceInfo;

    private void setUp() {
	android.net.wifi.WifiManager wifi = (android.net.wifi.WifiManager) getSystemService(android.content.Context.WIFI_SERVICE);
	lock = wifi.createMulticastLock("mylockthereturn");
	lock.setReferenceCounted(true);
	lock.acquire();
	try {
	    jmdns = JmDNS.create();
	    jmdns.addServiceListener(type, listener = new ServiceListener() {

		@Override
		public void serviceResolved(ServiceEvent ev) {
		    Inet4Address[] ip = ev.getInfo().getInet4Addresses();
		    // notifyUser("Service resolved: " + ev.getInfo().getName()
		    // + " ip:" + ip[0].toString());
		    if (ev.getInfo().getName().toString()
			    .startsWith("raspberry")) {
			loadUrl(ip[0].toString());
		    }
		    Log.d("SERVER", "@" + ev.getInfo().getName() + "@");

		}

		@Override
		public void serviceRemoved(ServiceEvent ev) {

		}

		@Override
		public void serviceAdded(ServiceEvent event) {
		    // Required to force serviceResolved to be called again
		    // (after the first search)
		    jmdns.requestServiceInfo(event.getType(), event.getName(),
			    1);
		}
	    });
	    /*
	     * serviceInfo = ServiceInfo.create("_test._tcp.local.",
	     * "AndroidTest", 0, "plain test service from android");
	     * jmdns.registerService(serviceInfo);
	     */
	} catch (IOException e) {
	    e.printStackTrace();
	    return;
	}
    }

    private void loadUrl(final String ip) {
	handler.postDelayed(new Runnable() {
	    public void run() {
		String url = "";
		if (ip.startsWith("/"))
		    url = ip.substring(1);
		else
		    url = ip;
		browserView = (WebView) findViewById(R.id.webkit);
		browserView.loadUrl("http://" + url + "/app.php");
	    }
	}, 1);

    }

    @Override
    protected void onStart() {
	super.onStart();
	// new Thread(){public void run() {setUp();}}.start();
    }

    @Override
    protected void onStop() {
	if (jmdns != null) {
	    if (listener != null) {
		jmdns.removeServiceListener(type, listener);
		listener = null;
	    }
	    jmdns.unregisterAllServices();
	    try {
		jmdns.close();
	    } catch (IOException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	    }
	    jmdns = null;
	}
	// repo.stop();
	// s.stop();
	if (lock != null)
	    lock.release();
	super.onStop();
    }
}