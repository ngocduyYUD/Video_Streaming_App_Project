import { useParams } from "react-router-dom";
import { useEffect } from "react";
import {useState} from "react";
 
export const Watch = () => {
  const { id } = useParams();
  const [src, setSrc] = useState(null);
  useEffect(() => {
    const params = {
      headers: { "content-type": "application/json" },
      body: JSON.stringify({ msg_type: "4", video_id: id }),
      method: "POST",
    };
    if (sessionStorage.getItem("vid-id " + id) == undefined){
        fetch("http://localhost:5500/", params)
        .then((res) => res.json())
        .then((res) => {
            console.log(res);
        //if (res.status === "ok") {
            //setSrc(res.path);
            sessionStorage.setItem("vid-id " + id, res.path);
            window.location.reload();
        //}
        });
    }else{
        setSrc(sessionStorage.getItem("vid-id " + id));
    }
    
  }, [id]);
  return (
    <div
      style={{
        display: "flex",
        justifyContent: "center",
      }}
    >
      <video
        id="vid-tag"
        controls
        autoplay
        src={src}
        style={{
          videoWidth: "60%",
          videoHeight: "60%",
        }}
      ></video>
    </div>
  );
  // .....
};