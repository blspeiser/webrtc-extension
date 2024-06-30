package io.cambium.webrtc

import android.content.Context
import io.cambium.webrtc.srtp.Aes256FrameDecryptor
import io.cambium.webrtc.srtp.Aes256FrameEncryptor
import org.webrtc.PeerConnection
import org.webrtc.PeerConnection.Observer
import org.webrtc.PeerConnectionFactory
import java.security.SecureRandom
import java.util.Collections
import javax.crypto.KeyGenerator

class ExamplePeerConnectionSetup {


    @OptIn(ExperimentalStdlibApi::class)
    fun initialize(applicationContext: Context) {
        //Encryption setup, yada yada yada
        val keygen = KeyGenerator.getInstance("AES")
        keygen.init(256);
        val key = keygen.generateKey()
        val iv = ByteArray(16)
        SecureRandom.getInstanceStrong().nextBytes(iv)
        val ivHex = iv.toHexString()

        //WebRTC Setup, yada yada yada
        val options = PeerConnectionFactory.InitializationOptions
            .builder(applicationContext)
            .createInitializationOptions()
        PeerConnectionFactory.initialize(options)
        val peerConnectionFactory = PeerConnectionFactory.builder()
            .createPeerConnectionFactory()

        val iceServer = PeerConnection
            .IceServer
            .builder("stun.voipzoom.com:3478") //public stun server
            .createIceServer()

        val rtcConnection = PeerConnection.RTCConfiguration(Collections.singletonList(iceServer))
        val peerConnection = peerConnectionFactory.createPeerConnection(rtcConnection, null as Observer?)

        //OK, now we have our peer connection! This is where we set our frame encryptor & decryptor!
        peerConnection?.senders?.filterNotNull()?.forEach {
            it.setFrameEncryptor( Aes256FrameEncryptor(key, ivHex) )
        }
        peerConnection?.receivers?.filterNotNull()?.forEach {
            it.setFrameDecryptor( Aes256FrameDecryptor(key, ivHex) )
        }

        //now do your streaming, have a nice day :)
    }
}