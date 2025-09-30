web4app.com. 3600 IN A 103.191.208.88  
mail.web4app.com. 3600 IN A 102.89.40.15  
  
web4app.com. 3600 IN MX 10 mail.web4app.com.  
  
web4app.com. 3600 IN NS ns1.web4app.com.  
web4app.com. 3600 IN NS ns2.web4app.com.  
; Optional NS from Herosite (uncomment only if you have authority over them)  
; web4app.com. 3600 IN NS win23.herosite.pro.  
; web4app.com. 3600 IN NS win24.herosite.pro.  
  
web4app.com. 3600 IN TXT "v=spf1 a mx include:relay.mailbaby.net include:relay.mailchannels.net a:Ultimate.herosite.pro ~all"  
  
_dmarc.web4app.com. 3600 IN TXT "v=DMARC1; p=none; rua=mailto:admin@web4app.com"  
  
; Optional DKIM entry — must match what your mail server generates  
; default._domainkey.web4app.com. 3600 IN TXT "v=DKIM1; k=rsa; p=MIIBIjANBgkqhkiG9w0BAQE...<rest-of-key>"  
  
; Optional: Allow only Let's Encrypt to issue certs  
web4app.com. 3600 IN CAA 0 issue "letsencrypt.org"  
