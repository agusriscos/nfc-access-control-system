package es.upm.b105.instrumentos105;

import android.app.Notification;
import android.app.NotificationManager;
import android.graphics.Color;
import android.media.RingtoneManager;
import android.net.Uri;
import android.nfc.cardemulation.HostApduService;
import android.os.Bundle;
import android.support.v7.app.NotificationCompat;
import android.util.Log;
import android.widget.Toast;

import java.util.Arrays;

import static es.upm.b105.instrumentos105.Utils.HexStringToByteArray;

/**
 * Created by Agustín Manuel on 02/03/2018.
 */

public class CardService extends HostApduService {

    private Utils utiles = new Utils();
    private static final String TAG = "CardService";
    // AID for our loyalty card service.
    private static final String AID = "F22222222200";
    // estado "OK" enviado en respuesta del comando SELECT AID (0x9000)
    private final byte[] SELECT_OK_SW = HexStringToByteArray("9000");
    // estado "UNKNOWN" enviado en respuesta de un APDU no valido (0x0000)
    private final byte[] UNKNOWN_CMD_SW = utiles.HexStringToByteArray("0000");
    //LOGIN_ERROR
    private final byte[] LOGIN_ERROR = utiles.HexStringToByteArray("4C4F47494E5F4552524F52");
    //SERVER_ERROR
    private final byte[] SERVER_ERROR = utiles.HexStringToByteArray("5345525645525F4552524F52");
    //GRANTED
    private final byte[] GRANTED = utiles.HexStringToByteArray("4752414E5445440A");
    //DENIED
    private final byte[] DENIED = utiles.HexStringToByteArray("44454E4945440A");
    private final byte[] SELECT_APDU = utiles.BuildSelectApdu(AID);


    /*
     * @param reason Either DEACTIVATION_LINK_LOSS or DEACTIVATION_DESELECTED
     */
    @Override
    public void onDeactivated(int reason) {
    }

    /*
     * @param commandApdu The APDU recibido por el dispositivo remoto
     * @param extras bundle que contiene datos extra. Puede ser null.
     * @return un array de bytes que contiene el APDU de respuesta, o null si no se puede enviar ningun APDU.
     */
    @Override
    public byte[] processCommandApdu(byte[] commandApdu, Bundle extras) {
        Log.i(TAG, "APDU recibido por el lector: " + utiles.ByteArrayToHexString(commandApdu));
        SharedPreferenceUtils pref = SharedPreferenceUtils.getInstance(this);
        String userName = pref.getValue("username", null);
        Toast.makeText(getApplicationContext(), userName, Toast.LENGTH_LONG).show();
        if (Arrays.equals(LOGIN_ERROR, commandApdu)) {
            showNotificationLogin();
        }

        if (Arrays.equals(SERVER_ERROR, commandApdu)) {
            showNotificationServer();
        }
        if (Arrays.equals(GRANTED, commandApdu)) {
            showNotificationGranted();
        }
        if (Arrays.equals(DENIED, commandApdu)) {
            showNotificationDenied();
        }
        if (userName != null) {
            if (Arrays.equals(SELECT_APDU, commandApdu)) {
                byte[] user_b = userName.getBytes();
                Log.i(TAG, "Enviando key: " + user_b);
                return utiles.ConcatArrays(user_b, SELECT_OK_SW);
            }
        }else {
            return UNKNOWN_CMD_SW; //Debe registrarse antes de entrar
        }
        return UNKNOWN_CMD_SW;
    }
    public void showNotificationServer() {
        Uri ring = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);
        Notification notification = new NotificationCompat.Builder(this)
                .setVibrate(new long[] { 1000, 1000, 1000, 1000, 1000 })
                .setLights(Color.RED, 500, 500)
                .setTicker("Error al conectar al servidor")
                .setSmallIcon(R.drawable.logonotif)
                .setContentTitle("Error al conectar con el servidor")
                .setContentText("Contacte con el responsable")
                .setAutoCancel(true)
                .setSound(ring)
                .build();

        NotificationManager notificationManager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
        notificationManager.notify(0, notification);
    }
    public void showNotificationGranted() {
        Uri ring = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);
        Notification notification = new NotificationCompat.Builder(this)
                .setVibrate(new long[] { 1000, 1000, 1000, 1000, 1000 })
                .setLights(Color.RED, 500, 500)
                .setTicker("Acceso permitido")
                .setSmallIcon(R.drawable.logonotif)
                .setContentTitle("Acceso permitido")
                .setContentText("Puedes pasar")
                .setAutoCancel(true)
                .setSound(ring)
                .build();

        NotificationManager notificationManager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
        notificationManager.notify(0, notification);
    }
    public void showNotificationDenied() {
        Uri ring = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);
        Notification notification = new NotificationCompat.Builder(this)
                .setVibrate(new long[] { 1000, 1000, 1000, 1000, 1000 })
                .setLights(Color.RED, 500, 500)
                .setTicker("Acceso denegado")
                .setSmallIcon(R.drawable.logonotif)
                .setContentTitle("Acceso denegado")
                .setContentText("No puedes pasar")
                .setAutoCancel(true)
                .setSound(ring)
                .build();

        NotificationManager notificationManager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
        notificationManager.notify(0, notification);
    }
    public void showNotificationLogin() {
        Uri ring = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION);
        Notification notification = new NotificationCompat.Builder(this)
                .setVibrate(new long[] { 1000, 1000, 1000, 1000, 1000 })
                .setLights(Color.RED, 500, 500)
                .setTicker("Debe registrarse antes de entrar")
                .setSmallIcon(R.drawable.logonotif)
                .setContentTitle("Debe iniciar sesión antes de entrar")
                .setContentText("Si ya lo ha hecho por favor actualice la tarjeta")
                .setAutoCancel(true)
                .setSound(ring)
                .build();

        NotificationManager notificationManager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
        notificationManager.notify(0, notification);
    }
}

